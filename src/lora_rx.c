#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "../include/driver/lora_driver.h"

extern void spi_close();

int main()
{
    lora_driver_init();

    lora_idle_mode();

    lora_set_frequency(433);
    lora_set_bandwidth(4);
    lora_set_coding_rate(8);
    lora_set_spreading_factor(12);
    lora_enable_crc();

    lora_receive_mode();

    uint8_t buf[8];
    uint8_t return_len;
    //uint8_t size = 1;
    bool received = false;

    while(1) {
        lora_received(&received);

        if(received) {
            lora_receive_packet(buf, &return_len, sizeof(buf));
            for(int i = 0; i < sizeof(buf); i++) {
                printf("Data received: 0x%02X\n", *(buf + i));
            }
            break; // TODO no break
        }
    }

    lora_sleep_mode();

    spi_close();
}
