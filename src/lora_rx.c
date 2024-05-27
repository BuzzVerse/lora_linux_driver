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

    lora_receive_mode();

    uint8_t buf[32]; // temporary buffer size
    uint8_t return_len;
    //uint8_t size = 1;
    bool received = false;

    // TODO make infinite receive loop
    while(1) {
        lora_received(&received);

        if(received) {
            lora_receive_packet(buf, &return_len, sizeof(buf));
            for(uint8_t i = 0x00; i < return_len; i++) {
                printf("Data received: 0x%02X\n", *(buf + i));
            }
            return_len = 0x00;
            received = false;
            //break; // TODO how to exit the loop and cleanly exit the program? (i. e. by executing spi_close()) 
        }
    }

    lora_sleep_mode();

    spi_close();
}
