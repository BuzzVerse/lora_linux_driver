#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "driver/lora_driver.h"
#include "packet/packet.h"

extern void spidev_close(); // function from lora_api_impl.c
extern int spidev_open(char* dev); // function from lora_api_impl.c

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

    if(spidev_open(device) == -1) {
        return -1; // exit if fd fails to open
    }

    temp_init();

    lora_idle_mode();

    lora_set_frequency(433 * 1e6);
    lora_set_bandwidth(4);
    lora_set_coding_rate(8);
    lora_set_spreading_factor(12);
    lora_enable_crc();

    // example packet data
    packet_t packet;
    packet.version = 0x01;
    packet.id = 0x00;
    packet.msgID = 0x01;
    packet.msgCount = 0x00;
    packet.dataType = 0x01;
    packet.data[0] = 25;
    packet.data[1] = 1;
    packet.data[2] = 50;

    lora_write_reg(REG_PAYLOAD_LENGTH, PACKET_SIZE);
    uint8_t payload_length;
    lora_read_reg(REG_PAYLOAD_LENGTH, &payload_length);

    //uint8_t* buf;
    //buf = (uint8_t*) calloc(payload_length, sizeof(uint8_t));

    for(uint8_t i = 0x00; i < payload_length; i++) { 
        //buf[(int)i] = i; // 0x00, 0x01, 0x02, 0x03
    }

    if (lora_send_packet((uint8_t*)&packet, PACKET_SIZE) == LORA_OK)
        printf("Packet sent successfully.\n"); // puts LoRa in sleep mode
                                               
    //free(buf);

    spidev_close();
}
