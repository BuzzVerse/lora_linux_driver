#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#include "driver/lora_driver.h"
#include "packet/packet.h"
#include "lora.h"

packet_t* packet = NULL;
uint8_t* buffer = NULL;

void handle_sigint(int sig) {
    printf("Caught signal %d (SIGINT), cleaning up...\n", sig);

    if(lora_sleep_mode() != LORA_OK) {
        printf("Failed to set sleep mode.\n");
    }

    if(packet != NULL) {
        free(packet);
    }

    if(buffer != NULL) {
        free(buffer);
    }

    spidev_close();

    exit(0);
}

int main(int argc, char* argv[]) {
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

    if(temp_init() == LORA_FAILED_INIT) {
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

    // example packet data
    packet = (packet_t*)malloc(sizeof(packet_t));
    packet->version = 0x33;
    packet->id = 0x22;
    packet->msgID = 0x11;
    packet->msgCount = 0x00;
    packet->dataType = BME280; 
    packet->data[0] = 23;
    packet->data[1] = 4;
    packet->data[2] = 56;
    
    size_t payload_size = (META_DATA_SIZE + get_data_size(packet->dataType));
    buffer = malloc(payload_size * sizeof(uint8_t));
    pack_packet(buffer, packet, payload_size);
    printf("Buffer to be sent:\n");
    print_buffer(buffer, payload_size);

    if(lora_write_reg(REG_PAYLOAD_LENGTH, PACKET_SIZE) != LORA_OK) {
        printf("Failed to set payload length\n");
        loginfo("Failed to set payload length\n");
        spidev_close();
        return -1;
    }

    if (lora_send_packet(buffer, payload_size) == LORA_OK) {  // puts LoRa in sleep mode
        printf("Packet sent successfully.\n");
        loginfo("Packet sent successfully.\n");
    } else {
        printf("Failed to send packet\n");
        loginfo("Failed to send packet\n");
    }

    free(packet);
    free(buffer);
    
    spidev_close();

    return 0;
}
