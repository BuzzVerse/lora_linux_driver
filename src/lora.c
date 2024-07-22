#include "lora.h"

// stuff for mocking functions for unit tests
#ifdef MOCK
#include "colors.h"
#include <stdlib.h>
lora_status_t mock_lora_receive_mode(void) {
    return LORA_OK;
}

#ifdef LORA_RECECIVED_TF
lora_status_t mock_lora_received1(bool *received, bool *crc_error) {
    *received = true;
    *crc_error = false;
    return LORA_OK;
}
#define lora_received mock_lora_received1
#endif

#ifdef LORA_RECECIVED_TT
lora_status_t mock_lora_received2(bool *received, bool *crc_error) {
    *received = true;
    *crc_error = true;
    return LORA_OK;
}
#define lora_received mock_lora_received2
#endif

#ifdef LORA_RECEIVE_PACKET_BME
lora_status_t mock_lora_receive_packet_bme(uint8_t *buf, uint8_t *return_len, uint8_t size) {
    buf[0] = 0x33; 
    buf[1] = 0x22;
    buf[2] = 0x11;
    buf[3] = 0x00;
    buf[4] = 0x01;
    buf[5] = 0x17;
    buf[6] = 0x04;
    buf[7] = 0x38;
    *return_len = size;
    return LORA_OK;
}
#define lora_receive_packet mock_lora_receive_packet_bme
#endif

#ifdef LORA_RECEIVE_PACKET_BMA
lora_status_t mock_lora_receive_packet_bma(uint8_t *buf, uint8_t *return_len, uint8_t size) {
    buf[0] = 0x33; 
    buf[1] = 0x22;
    buf[2] = 0x11;
    buf[3] = 0x00;
    buf[4] = 0x02;
    buf[5] = 0x17;
    buf[6] = 0x04;
    buf[7] = 0x38;
    *return_len = size;
    return LORA_OK;
}
#define lora_receive_packet mock_lora_receive_packet_bma
#endif

lora_status_t mock_lora_packet_rssi(uint8_t *rssi) {
    *rssi = 164;
    return LORA_OK;
}

lora_status_t mock_lora_packet_snr(uint8_t *snr) {
    *snr = 7;
    return LORA_OK;
}
#define lora_receive_mode mock_lora_receive_mode
#define lora_packet_rssi mock_lora_packet_rssi
#define lora_packet_snr mock_lora_packet_snr
#endif

void print_buffer(uint8_t* buf, uint8_t len) {
    for(uint8_t i = 0x00; i < len; i++) {
        printf("0x%02X ", *(buf + i));
    }
    printf("\n");
}

void buffer_to_string(uint8_t* buffer, size_t buffer_size, char* destination, size_t destination_size) {
    snprintf(destination, destination_size,
            "version: 0x%02X, id: 0x%02X, msgID: 0x%02X, msgCount: 0x%02X, dataType: 0x%02X, data: ",
            buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]);
    for(size_t i = META_DATA_SIZE; i < buffer_size; i++) {
        char minibuff[6];
        snprintf(minibuff, sizeof(minibuff), "0x%02X ", buffer[i]);
        strcat(destination, minibuff);
    }
}

int loginfo(const char* msg) {
    FILE* fptr;
    fptr = fopen(LOGFILE, "a");
    if(fptr == NULL) {
        printf("Error opening logfile\n");
        return -1;
    }

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char buf[64]; // mind the buffer size
    strftime(buf, sizeof(buf), "%y-%m-%d %H:%M:%S", tm);

    fprintf(fptr, "[%s] %s", buf, msg);

    fclose(fptr);
    return 0;
}

lora_status_t lora_receive(packet_t* packet) {
    uint8_t return_len = 0;
    uint8_t rssi = 0;
    uint8_t snr = 0;

    lora_receive_mode();

    while(1) {
        bool received = false;
        bool crc_error = false;
        lora_received(&received, &crc_error);

        if(received) {
            uint8_t buffer[PACKET_SIZE]; // buffer always of the maximum packet size //TODO does this buffer need to be zeroed every time?
            lora_receive_packet(buffer, &return_len, sizeof(buffer)); // puts LoRa in idle mode!!!

            // TODO print/log this?
            lora_packet_rssi(&rssi);
            lora_packet_snr(&snr);

            packet->version = buffer[0];
            packet->id = buffer[1];
            packet->msgID = buffer[2];
            packet->msgCount = buffer[3];
            packet->dataType = buffer[4];

            size_t payload_size = META_DATA_SIZE + get_data_size(packet->dataType);
            memcpy(packet->data, &buffer[META_DATA_SIZE], payload_size); // only pack the useful data

            return crc_error ? LORA_CRC_ERROR : LORA_OK;
        }
        lora_delay(20); // 20 ms
    }
}

size_t get_data_size(DataType type) {
    switch (type) {
    case BME280:
        return 3; // 1B temp, 1B hum, 1B pres
    case BMA400:
        return 24; // 8B x-axis, 8B y-axis, 8B z-axis
    case MQ2:
        return 17; // 1B gas type, 16B value
    case GPS:
        return 16; // 8B longitude, 8B latitude
    case SMS:
        return 59; // Max 59B String
    default:
        return 0; // Unsupported type
    }
}
