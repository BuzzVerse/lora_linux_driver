#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "../include/driver/lora_driver.h"

extern void spidev_close(); // function from lora_api_impl.c
extern void spidev_open(char* dev); // function from lora_api_impl.c

int main()
{
    spidev_open("/dev/spidev0.0");

    lora_driver_init();

    lora_idle_mode();

    //lora_set_frequency(433); // TODO use this function both in TX & RX and see if they work
    lora_set_bandwidth(4);
    lora_set_coding_rate(8);
    lora_set_spreading_factor(12);
    lora_enable_crc();

    //lora_write_reg(REG_PAYLOAD_LENGTH, 0x04);

    uint8_t buf[4] = {0x10, 0x11, 0x12, 0x13}; // TODO change size to PAYLOAD_LENGTH
    if (lora_send_packet(buf, sizeof(buf)) == LORA_OK)
        printf("Packet sent successfully.\n"); // puts LoRa in sleep mode

    //lora_sleep_mode();

    spidev_close();
}
