#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <string.h>

#include "colors.h"
#include "driver/lora_driver.h"
#include "api/driver_api.h"

#define MESSAGE_SIZE    10

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

void buffer_to_string(uint8_t* buffer, char* destination) {
    snprintf(destination, 128, "0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
            buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9]);
}

lora_status_t lora_receive(uint8_t* data) {
    uint8_t buffer[MESSAGE_SIZE] = {0};
    uint8_t return_len = 0;

    lora_receive_mode();

    while(1) {
        bool received = false;
        bool crc_error = false;
        lora_received(&received, &crc_error);

        if(received) {
            lora_receive_packet(buffer, &return_len, sizeof(buffer)); // puts LoRa in idle mode!!!
            
            memcpy(data, buffer, MESSAGE_SIZE);

            return crc_error ? LORA_CRC_ERROR : LORA_OK;
        }
        lora_delay(20); // 20 ms
    }
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

    while(1) {
        uint8_t data[MESSAGE_SIZE] = {0};

        if(lora_receive(data) == LORA_OK) { // waits for a packet and writes it to a buffer on its arrival
            char raw_data[64];
            buffer_to_string(data, raw_data);

            char message[128];
            snprintf(message, sizeof(message), "Raw data: %s\nDecoded data: %s\n", raw_data, (char*)data);
            printf("%s", message);
            loginfo(message);
        } else {
            printf("%s[ERROR]%s CRC error \n", C_RED, C_DEFAULT);
            loginfo("[ERROR] CRC error \n");
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
