#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <string.h>

#include "driver/lora_driver.h"
#include "packet/packet.h"
#include "lora.h"

packet_t* packet = NULL;

void handle_sigint(int sig) {
    printf("Caught signal %d (SIGINT), cleaning up...\n", sig);

    if(lora_sleep_mode() != LORA_OK) {
        printf("Failed to set sleep mode.\n");
    }

    if(packet != NULL) {
        free(packet);
    }

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
        return -1;
    }

    // signal handler for CTRL-C
    signal(SIGINT, handle_sigint);

    if(spidev_open(device) == -1) {
        printf("Failed to open SPI device\n");
        loginfo("Failed to open SPI device\n");
        return -1; // exit if fd fails to open
    }

    if(lora_driver_init() == LORA_FAILED_INIT) {
        printf("Failed to initialize the driver\n");
        loginfo("Failed to initialize the driver\n");
        spidev_close();
        return -1;
    }

    lora_status_t ret;
    ret = lora_set_frequency(433 * 1e6);
    ret += lora_set_bandwidth(4);
    ret += lora_set_coding_rate(8);
    ret += lora_set_spreading_factor(12);
    ret += lora_enable_crc();
    if(ret != LORA_OK) {
        printf("Failed to set radio parameters\n");
        loginfo("Failed to set radio parameters\n");
        spidev_close();
        return -1;
    }

    if(lora_receive_mode() != LORA_OK) {
        printf("Failed to set receive mode\n");
        loginfo("Failed to set receive mode\n");
        spidev_close();
        return -1;
    }

    uint8_t buffer[PACKET_SIZE];
    packet = (packet_t*)malloc(sizeof(packet_t));
    bool received = false;
    bool crc_error = false;
    uint8_t irq;

    while(1) {
        received = false;
        crc_error = false;
        lora_received(&received, &crc_error);

        if(received) {
            lora_get_irq(&irq);
            char msg[100];

            if(crc_error) {
                printf("CRC error\n");
                loginfo("CRC error\n");
            } else {
                uint8_t return_len;
                lora_receive_packet(buffer, &return_len, PACKET_SIZE); // puts LoRa in idle mode!!!
                
                print_buffer(buffer, sizeof(buffer));
               
                packet->version = buffer[0];
                packet->id = buffer[1];
                packet->msgID = buffer[2];
                packet->msgCount = buffer[3];
                packet->dataType = buffer[4];
                memcpy(packet->data, &buffer[META_DATA_SIZE], DATA_SIZE);

                char raw_data[1024];
                buffer_to_string(buffer, raw_data);
                loginfo(raw_data); // logging raw received data

                // unpack data
                float received_temp = ((float)((int8_t)packet->data[0]) / 2.0);
                float received_press = (float)(1000 + (int8_t)packet->data[1]);
                float received_hum = (float)packet->data[2];

                // write message
                sprintf(msg, "{\"temperature\":%.2f, \"pressure\":%.2f, \"humidity\":%.2f}",
                        received_temp, received_press, received_hum);

                loginfo(strcat(msg, "\n")); // logging unpacked data

                lora_receive_mode();
            }
        }
    }
    
    if(lora_sleep_mode() != LORA_OK) {
        printf("Failed to set sleep mode\n");
        loginfo("Failed to set sleep mode\n");
        spidev_close();
        return -1;
    }

    free(packet);

    spidev_close();

    return 0;
}
