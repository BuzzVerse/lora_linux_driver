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

// functions from bbb_api_impl.c // TODO cleanup and move them elsewhere?
extern void spidev_close();
extern int spidev_open(char* dev);
extern void print_buffer(uint8_t* buf, uint8_t len);
extern int loginfo(const char* msg);

void handle_sigint(int sig) {
    printf("Caught signal %d (SIGINT), cleaning up...\n", sig);

    if(lora_sleep_mode() != LORA_OK) {
        printf("Failed to set sleep mode.\n");
    }

    spidev_close();

    exit(0);
}

void packet_to_string(packet_t packet, char* destination) {
    snprintf(destination, 100,
            "version: 0x%02X, id: 0x%02X, msgID: 0x%02X, msgCount: 0x%02X, dataType: 0x%02X, data: [0x%02X, 0x%02X, 0x%02X]\n",
            packet.version, packet.id, packet.msgID, packet.msgCount, packet.dataType, packet.data[0], packet.data[1], packet.data[2]);
}

void buffer_to_string(uint8_t* buffer, char* destination) {
    snprintf(destination, 100,
            "version: 0x%02X, id: 0x%02X, msgID: 0x%02X, msgCount: 0x%02X, dataType: 0x%02X, data: [0x%02X, 0x%02X, 0x%02X]\n",
            buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7]);
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
    packet_t *packet;
    bool received = false;
    bool crc_error = false;
    uint8_t irq;

    while(1) {
        lora_received(&received, &crc_error);

        if(received) {
            lora_get_irq(&irq);
            char msg[100];

            if(crc_error) {
                printf("CRC error\n");
                loginfo("CRC error\n");
                crc_error = false;
            } else {
                uint8_t return_len;
                //lora_receive_packet((uint8_t*)&packet, &return_len, PACKET_SIZE); // puts LoRa in idle mode!!!
                lora_receive_packet(buffer, &return_len, PACKET_SIZE); // puts LoRa in idle mode!!!
                
                print_buffer(buffer, sizeof(buffer));
               
                packet->version = buffer[0];
                packet->id = buffer[1];
                packet->msgID = buffer[2];
                packet->msgCount = buffer[3];
                packet->dataType = buffer[4];
                memcpy(packet->data, &buffer[META_DATA_SIZE], DATA_SIZE);

                char raw_data[100];
                //packet_to_string(*packet, raw_data);
                buffer_to_string(buffer, raw_data);
                loginfo(raw_data); // logging raw received data

                // unpack data
                float received_temp = ((float)((int8_t)packet->data[0]) / 2.0);
                float received_press = (float)(1000 + (int8_t)packet->data[1]);
                float received_hum = (float)packet->data[2];

                // write message
                sprintf(msg, "{\"temperature\":%.2f, \"pressure\":%.2f, \"humidity\":%.2f}",
                        received_temp, received_press, received_hum);

                loginfo(msg); // logging unpacked data

                lora_receive_mode();
            }
            
            received = false;
        }
    }
    
    if(lora_sleep_mode() != LORA_OK) {
        printf("Failed to set sleep mode\n");
        loginfo("Failed to set sleep mode\n");
        spidev_close();
        return -1;
    }

    spidev_close();

    return 0;
}
