#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "../include/driver/lora_driver.h"

extern void spi_close();

int main()
{
    lora_driver_init();

    lora_idle_mode();

    lora_receive_mode();

    while(1) {
        uint8_t buf;
        uint8_t return_len;
        uint8_t size = 1;
        lora_receive_packet(&buf, &return_len, size);

        if(lora_received(NULL) == LORA_OK) {
            printf("Data received: 0x%02X", buf);
            break; // TODO no break
        }
    }

    lora_sleep_mode();

    spi_close();
}
