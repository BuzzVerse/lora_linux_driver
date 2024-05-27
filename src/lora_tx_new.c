#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "../include/driver/lora_driver.h"

extern void spi_close();

int main()
{
    lora_driver_init();

    lora_idle_mode();

    //lora_set_frequency(433); // TODO use this function both in TX & RX and see if they work
    lora_set_bandwidth(4);
    lora_set_coding_rate(8);
    lora_set_spreading_factor(12);
    lora_enable_crc();

    uint8_t buf[4] = {0x10, 0x11, 0x12, 0x13};
    lora_send_packet(buf, sizeof(buf));

    lora_sleep_mode();

    spi_close();
}
