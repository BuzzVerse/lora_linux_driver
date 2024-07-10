#include "lora.h"

void print_buffer(uint8_t* buf, uint8_t len) {
    for(uint8_t i = 0x00; i < len; i++) {
        printf("0x%02X ", *(buf + i));
    }
    printf("\n");
}

void buffer_to_string(uint8_t* buffer, size_t buffer_size, char* destination) {
    snprintf(destination, 1024,
            "version: 0x%02X, id: 0x%02X, msgID: 0x%02X, msgCount: 0x%02X, dataType: 0x%02X, data: ",
            buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]);
    for(size_t i = META_DATA_SIZE; i < buffer_size; i++) {
        char* minibuff;
        snprintf(minibuff, 6, "0x%02X ", buffer[i]);
        strcat(destination, minibuff);
    }
    strcat(destination, "\n");
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
    uint8_t buffer[PACKET_SIZE] = {0};
    uint8_t return_len = 0;
    uint8_t rssi = 0;
    uint8_t snr = 0;

    lora_receive_mode();

    while(1) {
        bool received = false;
        bool crc_error = false;
        lora_received(&received, &crc_error);

        if(received) {
            lora_receive_packet(buffer, &return_len, sizeof(buffer)); // puts LoRa in idle mode!!!

            lora_packet_rssi(&rssi);
            lora_packet_snr(&snr);
    
            packet->version = buffer[0];
            packet->id = buffer[1];
            packet->msgID = buffer[2];
            packet->msgCount = buffer[3];
            packet->dataType = buffer[4];

            memcpy(packet->data, &buffer[META_DATA_SIZE], DATA_SIZE);

            return crc_error ? LORA_CRC_ERROR : LORA_OK;
        }
        lora_delay(20); // 20 ms
    }
}

// temporary driver init function without hardware init
lora_status_t temp_init(void) {
   lora_status_t ret;

   uint8_t version;
   uint8_t i = 0;
   while (i++ < TIMEOUT_RESET)
   {
      lora_read_reg(REG_VERSION, &version);
      printf("version=0x%02x\n", version);
      if (version == 0x12)
         break;
      sleep(20);
   }
   printf("i=%d, TIMEOUT_RESET=%d", i, TIMEOUT_RESET);

   if (i == TIMEOUT_RESET + 1)
      return LORA_FAILED_INIT;

   ret = lora_sleep_mode();
   ret += lora_write_reg(REG_FIFO_RX_BASE_ADDR, 0);
   ret += lora_write_reg(REG_FIFO_TX_BASE_ADDR, 0);
   uint8_t lna_val;
   lora_read_reg(REG_LNA, &lna_val);
   ret += lora_write_reg(REG_LNA, lna_val | 0x03);
   ret += lora_write_reg(REG_MODEM_CONFIG_3, 0x04);
   ret += lora_set_tx_power(17);

   ret += lora_idle_mode();

   return ret;
}

void pack_packet(uint8_t *buffer, packet_t *packet, size_t data_size) {
    buffer[PACKET_VERSION_IDX] = packet->version;
    buffer[PACKET_ID_IDX] = packet->id;
    buffer[PACKET_MSG_ID_IDX] = packet->msgID;
    buffer[PACKET_MSG_COUNT_IDX] = packet->msgCount;
    buffer[PACKET_DATA_TYPE_IDX] = packet->dataType;
    memcpy(&buffer[META_DATA_SIZE], packet->data, data_size);
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
