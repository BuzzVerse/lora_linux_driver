#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include "mqtt/mqtt.h"
#include "driver/lora_driver.h"
#include "packet/packet.h"

// functions from bbb_api_impl.c
extern void spidev_close();
extern void spidev_open(char* dev);
extern void print_buffer(uint8_t* buf, uint8_t len);

void handle_sigint(int sig) {
    printf("Caught signal %d (SIGINT), cleaning up...\n", sig);

    lora_sleep_mode();

    spidev_close();

    exit(0);
}

int main(int argc, char* argv[])
{
    if(argc != 2) {
        printf("Invalid number of arguments\nUsage: sudo ./lora_rx [SPI device number: 0 | 1]\n");
        return -1;
    }

    char* device;

    if(strcmp(argv[1], "0") == 0) {
        device = "/dev/spidev0.0";
    } else if(strcmp(argv[1], "1") == 0) {
        device = "/dev/spidev1.0";
    } else {
        printf("Invalid SPI device\n");
        return -2;
    }

    // signal handler for CTRL-C
    signal(SIGINT, handle_sigint);

    spidev_open(device);

    lora_driver_init();

    lora_idle_mode();

    lora_set_frequency(433 * 1e6);
    lora_set_bandwidth(4);
    lora_set_coding_rate(8);
    lora_set_spreading_factor(12);
    lora_enable_crc();

    lora_receive_mode();

    packet_t packet;
    bool received = false;
    bool crc_error = false;
    uint8_t irq;

    while(1) {
        lora_received(&received, &crc_error);

        if(received) {
            lora_get_irq(&irq);

            if(crc_error) {
                printf("CRC error\n");
                crc_error = false;
            } else {
                uint8_t return_len;
                lora_receive_packet((uint8_t*)&packet, &return_len, PACKET_SIZE); // puts LoRa in idle mode!!!
                print_buffer((uint8_t*)&packet, return_len);
                lora_receive_mode();
            }
            
            received = false;
        }
    }

    lora_sleep_mode();

    spidev_close();
}
