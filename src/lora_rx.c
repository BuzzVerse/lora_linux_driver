#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include "driver/lora_driver.h"

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

int main()
{
    // signal handler for CTRL-C
    signal(SIGINT, handle_sigint);

    spidev_open("/dev/spidev1.0");

    lora_driver_init();

    lora_idle_mode();

    lora_set_frequency(433);
    lora_set_bandwidth(4);
    lora_set_coding_rate(8);
    lora_set_spreading_factor(12);
    lora_enable_crc();

    lora_receive_mode();

    uint8_t buf[32]; // temporary buffer size
    bool received = false;
    bool crc_error = false;
    uint8_t irq;

    while(1) {
        lora_received(&received, &crc_error);

        if(received) {
            lora_get_irq(&irq);

            if(crc_error) {
                printf("CRC error.\n");
                crc_error = false;
            } else {
                uint8_t return_len;
                lora_receive_packet(buf, &return_len, sizeof(buf)); // puts LoRa in idle mode!!!
                print_buffer(buf, return_len);
                lora_receive_mode();
            }
            
            received = false;
        }
    }

    lora_sleep_mode();

    spidev_close();
}
