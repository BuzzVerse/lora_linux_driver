#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "driver/lora_driver.h"

extern void spidev_close(); // function from lora_api_impl.c
extern void spidev_open(char* dev); // function from lora_api_impl.c

lora_status_t temp_init(void)
{
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

int main()
{
    spidev_open("/dev/spidev0.0");

    temp_init();

    lora_idle_mode();

    lora_set_frequency(433);
    lora_set_bandwidth(4);
    lora_set_coding_rate(8);
    lora_set_spreading_factor(12);
    lora_enable_crc();

    lora_write_reg(REG_PAYLOAD_LENGTH, 0x04); // temporary size
    uint8_t payload_length;
    lora_read_reg(REG_PAYLOAD_LENGTH, &payload_length);

    uint8_t* buf;
    buf = (uint8_t*) calloc(payload_length, sizeof(uint8_t));

    for(uint8_t i = 0x00; i < payload_length; i++) { 
        buf[(int)i] = i; // 0x00, 0x01, 0x02, 0x03
    }

    if (lora_send_packet(buf, sizeof(buf)) == LORA_OK)
        printf("Packet sent successfully.\n"); // puts LoRa in sleep mode
                                               
    free(buf);

    spidev_close();
}