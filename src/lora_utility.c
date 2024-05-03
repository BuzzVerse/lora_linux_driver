#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "lora_utility.h"
#include "lora_registers.h"
#include "spi_io.h"

void lora_initialize(int fd) {
    spi_write_register(fd, FIFO_RX_BASE_ADDR, 0x00);    
    spi_write_register(fd, FIFO_TX_BASE_ADDR, 0x00);    
	spi_write_register(fd, LNA, spi_read_register(fd, LNA) | 0x03);
	spi_write_register(fd, MODEM_CONFIG_3, 0x04);
    spi_write_register(fd, PA_CONFIG, 0x8F);
    spi_write_register(fd, MODEM_CONFIG_1, 0x48); // BW = 4, CR = 4/8
    spi_write_register(fd, MODEM_CONFIG_2, 0xC4); // SF = 12, CRC enabled
}

void print_modem_status(int fd) {
    uint8_t modem_status = spi_read_register(fd, MODEM_STATUS);
	sleep((double)0.1);
    if((modem_status & 0x10) == 0x10) { printf("    [MODEM_STATUS] Modem clear\n"); }
    if((modem_status & 0x08) == 0x08) { printf("    [MODEM_STATUS] Header info valid\n"); }
    if((modem_status & 0x04) == 0x04) { printf("    [MODEM_STATUS] RX on-going\n"); }
    if((modem_status & 0x02) == 0x02) { printf("    [MODEM_STATUS] Signal synchronized\n"); }
    if((modem_status & 0x01) == 0x01) { printf("    [MODEM_STATUS] Signal detected\n"); }
}

void print_irq_flags(int fd) {
    uint8_t irq_flags = spi_read_register(fd, IRQ_FLAGS);
	sleep((double)0.1);
    if((irq_flags & 0x80) == 0x80) { printf("[IRQ_FLAGS] RxTimeout\n"); }
    if((irq_flags & 0x40) == 0x40) { printf("[IRQ_FLAGS] RxDone\n"); }
    if((irq_flags & 0x20) == 0x20) { printf("[IRQ_FLAGS] PayloadCrcError\n"); }
    if((irq_flags & 0x10) == 0x10) { printf("[IRQ_FLAGS] ValidHeader\n"); }
    if((irq_flags & 0x08) == 0x08) { printf("[IRQ_FLAGS] TxDone\n"); }
    if((irq_flags & 0x04) == 0x04) { printf("[IRQ_FLAGS] CadDone\n"); }
    if((irq_flags & 0x02) == 0x02) { printf("[IRQ_FLAGS] FhssChangeChannel\n"); }
    if((irq_flags & 0x01) == 0x01) { printf("[IRQ_FLAGS] CadDetected\n"); }
}
