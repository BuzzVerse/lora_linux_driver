#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "lora_utility.h"
#include "lora_registers.h"
#include "spi_io.h"
#include "colors.h"

void lora_initialize(int fd) {
    spi_write_register(fd, FIFO_RX_BASE_ADDR, 0x00);    
    spi_write_register(fd, FIFO_TX_BASE_ADDR, 0x00);    
	spi_write_register(fd, LNA, spi_read_register(fd, LNA) | 0x03);
	spi_write_register(fd, MODEM_CONFIG_3, 0x04);
    spi_write_register(fd, PA_CONFIG, 0x8F);
    spi_write_register(fd, MODEM_CONFIG_1, 0x48); // BW = 4, CR = 4/8
    spi_write_register(fd, MODEM_CONFIG_2, 0xC4); // SF = 12, CRC enabled

    // Set frequency to 433 Hz
    //double frequency = 433;

    //uint64_t frf = ((uint64_t)frequency << 19) / 32000000;

    //spi_write_register(fd, FR_MSB, (uint8_t)(frf >> 16));
    //spi_write_register(fd, FR_MID, (uint8_t)(frf >> 8));
    //spi_write_register(fd, FR_LSB, (uint8_t)(frf >> 0));
    
    spi_write_register(fd, FR_MSB, 0x6C);
    spi_write_register(fd, FR_MID, 0x40);
    spi_write_register(fd, FR_LSB, 0x00);
}

void lora_dump_registers(int fd)
{
   uint8_t i;
   printf("00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
   for (i = 0; i < 0x40; i++)
   {
      printf("%02X ", spi_read_register(fd, i));
      if ((i & 0x0f) == 0x0f)
         printf("\n");
   }
   printf("\n");
}

void set_mode(int fd, uint8_t mode) {
    spi_write_register(fd, OP_MODE, mode);
}

void set_bandwidth(int fd, int bw) {
    if(bw < 0) { bw = 0; }
    else if(bw > 9) { bw = 9; }
    
    spi_write_register(fd, MODEM_CONFIG_1, spi_read_register(fd, MODEM_CONFIG_1) | (bw << 4));
}

void set_coding_rate(int fd, int cr) {
    if(cr < 1) { cr = 1; }
    else if(cr > 4) { cr = 4; }

    spi_write_register(fd, MODEM_CONFIG_1, spi_read_register(fd, MODEM_CONFIG_1) | (cr << 1));
}

void set_spreading_factor(int fd, int sf) {
    if(sf < 6) { sf = 6; }
    else if(sf > 12) { sf = 12; }

    spi_write_register(fd, MODEM_CONFIG_2, spi_read_register(fd, MODEM_CONFIG_2) | (sf << 4));
}

void enable_crc(int fd) {
    spi_write_register(fd, MODEM_CONFIG_2, spi_read_register(fd, MODEM_CONFIG_2) | (1 << 2));
}

void print_modem_status(int fd) {
    uint8_t modem_status = spi_read_register(fd, MODEM_STATUS);
    if((modem_status & 0x10) == 0x10) { printf("    %s[MODEM_STATUS]%s Modem clear\n", C_YELLOW, C_DEFAULT); }
    if((modem_status & 0x08) == 0x08) { printf("    %s[MODEM_STATUS]%s Header info valid\n", C_YELLOW, C_DEFAULT); }
    if((modem_status & 0x04) == 0x04) { printf("    %s[MODEM_STATUS]%s RX on-going\n", C_YELLOW, C_DEFAULT); }
    if((modem_status & 0x02) == 0x02) { printf("    %s[MODEM_STATUS]%s Signal synchronized\n", C_YELLOW, C_DEFAULT); }
    if((modem_status & 0x01) == 0x01) { printf("    %s[MODEM_STATUS]%s Signal detected\n", C_YELLOW, C_DEFAULT); }
}

void print_irq_flags(int fd) {
    uint8_t irq_flags = spi_read_register(fd, IRQ_FLAGS);
    if((irq_flags & 0x80) == 0x80) { printf("%s[IRQ_FLAGS]%s RxTimeout\n", C_YELLOW, C_DEFAULT); }
    if((irq_flags & 0x40) == 0x40) { printf("%s[IRQ_FLAGS]%s RxDone\n", C_YELLOW, C_DEFAULT); }
    if((irq_flags & 0x20) == 0x20) { printf("%s[IRQ_FLAGS]%s PayloadCrcError\n", C_YELLOW, C_DEFAULT); }
    if((irq_flags & 0x10) == 0x10) { printf("%s[IRQ_FLAGS]%s ValidHeader\n", C_YELLOW, C_DEFAULT); }
    if((irq_flags & 0x08) == 0x08) { printf("%s[IRQ_FLAGS]%s TxDone\n", C_YELLOW, C_DEFAULT); }
    if((irq_flags & 0x04) == 0x04) { printf("%s[IRQ_FLAGS]%s CadDone\n", C_YELLOW, C_DEFAULT); }
    if((irq_flags & 0x02) == 0x02) { printf("%s[IRQ_FLAGS]%s FhssChangeChannel\n", C_YELLOW, C_DEFAULT); }
    if((irq_flags & 0x01) == 0x01) { printf("%s[IRQ_FLAGS]%s CadDetected\n", C_YELLOW, C_DEFAULT); }
}

void lora_reset(void) {
    for (int i = 0x0; i < 0x2; i++) {
        FILE *fptr66, *fptr69;
        fptr66 = fopen("/sys/class/gpio/gpio66/value", "w");
        fptr69 = fopen("/sys/class/gpio/gpio69/value", "w");

        if ((fptr66 == NULL) || (fptr69 == NULL)) {
            printf("%s[Error]%s Reading GPIO failed.\n", C_RED, C_DEFAULT);
            exit(1);
        }

        fprintf(fptr66, "%d", i);
        fprintf(fptr69, "%d", i);

        sleep((double)(0.0001));

        fclose(fptr66);
        fclose(fptr69);
    }
    sleep((double)(0.005));
    printf("%s[RESET]%s Ok", C_GREEN, C_DEFAULT);
}
