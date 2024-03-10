#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "lora_registers.h"

#define SPI_SPEED_HZ	500000
#define SPI_WRITE	0x80
#define SPI_READ	0x00

/**
 * Function to write data to register
 * @param fd - file descriptor
 * @param reg - register address
 * @param value - value to write
 */
void spi_write_register(int fd, uint8_t reg, uint8_t value) {
    uint8_t tx[] = {reg | SPI_WRITE, value};
    uint8_t rx[2] = {0, 0};
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = 2,
        .speed_hz = SPI_SPEED_HZ,
        .bits_per_word = 8,
    };

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 0) {
        perror("SPI Write Error");
    }
}

/**
 * Function to read data from register
 * @param fd - file descriptor
 * @param reg - register address
 * @return - value from register
 */
uint8_t spi_read_register(int fd, uint8_t reg) {
    uint8_t tx[] = {reg | SPI_READ, 0x00}; // We are sending register address and 0x00 to push out data from slave
    uint8_t rx[2] = {0, 0};
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = 2,
        .speed_hz = SPI_SPEED_HZ,
        .bits_per_word = 8,
    };

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 0) {
        perror("SPI Read Error");
        return 0;
    }

    return rx[1]; // Return second byte from rx buffer that is read value from register
}

int main()
{
	int fd = open("/dev/spidev0.0", O_RDWR);
    	if (fd == 0) {
        	perror("Can't open device. Check permissions and if file exists");
        	return 1;
    	}
	
	printf("[Before setting] OP_MODE: 0x%02X\n", spi_read_register(fd, OP_MODE));

	// Set LoRa Sleep mode
	printf("Setting LORA_SLEEP...");
	spi_write_register(fd, OP_MODE, LORA_SLEEP); // there is a small delay between calling spi_write_register() and the register value being actually written, hence the need to wait for a while
	while(spi_read_register(fd, OP_MODE) != LORA_SLEEP) {
		printf(".");
	}
	printf(" LORA_SLEEP set.\n");

	// Set LoRa Standby mode
	printf("Setting LORA_STANDBY...");
	spi_write_register(fd, OP_MODE, LORA_STANDBY); 
	while(spi_read_register(fd, OP_MODE) != LORA_STANDBY) {
		printf(".");
	}
	printf(" LORA_STANDBY set.\n");

	printf("[After setting] OP_MODE: 0x%02X\n", spi_read_register(fd, OP_MODE));

	// FifoAddrPtr located under 0x0D (default value: 0x00)
	printf("FIFO_ADDR_PTR: 0x%02X\n", spi_read_register(fd, FIFO_ADDR_PTR));

	// FifoTxBaseAddr located under 0x0E (default value 0x80)
	printf("FIFO_TX_BASE_ADDR: 0x%02X\n", spi_read_register(fd, FIFO_TX_BASE_ADDR));

	// Fill the FIFO with data to transmit:
	// 1) Set FifoPtrAddr to FifoTxPtrBase
	spi_write_register(fd, FIFO_ADDR_PTR, spi_read_register(fd, FIFO_TX_BASE_ADDR));
	printf("[After setting] FIFO_ADDR_PTR: 0x%02X\n", spi_read_register(fd, FIFO_ADDR_PTR));

	// 2) Write 4 bytes to the FIFO
	// TODO rethink
	for(int i = 0; i < 4; i++) {
		spi_write_register(fd, FIFO, 0xAF);
		printf("[Writing...: %d] FIFO_ADDR_PTR: 0x%02X\n", i, spi_read_register(fd, FIFO_ADDR_PTR));
	}
	printf("[After writing to FIFO] FIFO_ADDR_PTR: 0x%02X\n", spi_read_register(fd, FIFO_ADDR_PTR));

	// 1) Set FifoPtrAddr to FifoTxPtrBase
	spi_write_register(fd, FIFO_ADDR_PTR, spi_read_register(fd, FIFO_TX_BASE_ADDR));
	printf("[After setting] FIFO_ADDR_PTR: 0x%02X\n", spi_read_register(fd, FIFO_ADDR_PTR));

	for(uint8_t i = 0; i < 4; i++) {
		printf("[Reading...: %d] FIFO_ADDR_PTR: 0x%02X, FIFO data: 0x%02X\n", i, spi_read_register(fd, FIFO_ADDR_PTR), spi_read_register(fd, FIFO));
	}

	// Set TX mode to initiate data transmission
	// CAUTION: DO NOT initiate transmission unless antenna is attached to LoRa
	// spi_write_register(fd, OP_MODE, LORA_TX);
	// TODO wait for some time? maybe check the TxDone interrupt?

	// Put LoRa in Sleep mode
	printf("Setting LORA_SLEEP...");
	spi_write_register(fd, OP_MODE, LORA_SLEEP);
	while(spi_read_register(fd, OP_MODE) != LORA_SLEEP) {
		printf(".");
	}
	printf(" LORA_SLEEP set.\n");

	close(fd);

	return 0;
}
