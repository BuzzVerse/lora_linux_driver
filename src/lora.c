#include "lora.h"


void print_buffer(uint8_t* buf, uint8_t len) {
    for(uint8_t i = 0x00; i < len; i++) {
        printf("0x%02X ", *(buf + i));
    }
    printf("\n");
}

void buffer_to_string(uint8_t* buffer, char* destination) {
    snprintf(destination, 128, "0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X",
            buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9]);
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
