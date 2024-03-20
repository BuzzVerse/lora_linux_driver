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
	
	spi_write_register(fd, OP_MODE, 0x01);

	// Set LoRa Sleep mode
	printf("Setting LORA_SLEEP...");
	//spi_write_register(fd, OP_MODE, LORA_SLEEP);
	spi_write_register(fd, OP_MODE, 0x80);
	// there is a small delay between calling spi_write_register()
	// and the register value being actually written, 
	// hence the need to wait for a while
	while(spi_read_register(fd, OP_MODE) != 0x80) {}
	printf(" LORA_SLEEP set [OP_MODE: 0x%02X]\n", spi_read_register(fd, OP_MODE));

	spi_write_register(fd, LNA, 0x23);
	spi_write_register(fd, MODEM_CONFIG_3, 0x04);
	spi_write_register(fd, PA_CONFIG, 0x8F);
	spi_write_register(fd, FR_LSB, 0x6C);
	spi_write_register(fd, FR_MID, 0x40);
	spi_write_register(fd, FR_MSB, 0x00);
	spi_write_register(fd, MODEM_CONFIG_2, 0x74);
	spi_write_register(fd, MODEM_CONFIG_1, 0x72);
	spi_write_register(fd, DETECT_OPTIMIZE, 0xC3);
	spi_write_register(fd, DETECTION_THRESHOLD, 0x0A);
	spi_write_register(fd, MODEM_CONFIG_2, 0x74);

	// Set LoRa Standby mode
	printf("Setting LORA_STANDBY...");
	spi_write_register(fd, OP_MODE, 0x81); 
	while(spi_read_register(fd, OP_MODE) != 0x81) {}
	printf(" LORA_STANDBY set [OP_MODE: 0x%02X]\n", spi_read_register(fd, OP_MODE));

	// Fill the FIFO with data to transmit:
	// 1) Set FifoPtrAddr to FifoTxPtrBase
	spi_write_register(fd, FIFO_ADDR_PTR, spi_read_register(fd, FIFO_TX_BASE_ADDR));
	printf("[After setting] FIFO_ADDR_PTR: 0x%02X\n", spi_read_register(fd, FIFO_ADDR_PTR));

	spi_write_register(fd, PAYLOAD_LENGTH, 0x0F);
	printf("PAYLOAD_LENGTH: 0x%02X \n", spi_read_register(fd, PAYLOAD_LENGTH));

	// 2) Write PAYLOAD_LENGTH bytes to the FIFO
	for(uint8_t i = 0x00; i < spi_read_register(fd, PAYLOAD_LENGTH); i++) {
		printf("[%d] Writing to FIFO_ADDR_PTR: 0x%02X\n", i, spi_read_register(fd, FIFO_ADDR_PTR));
		spi_write_register(fd, FIFO, 0xAF);
	}
	//printf("[After writing to FIFO] FIFO_ADDR_PTR: 0x%02X\n", spi_read_register(fd, FIFO_ADDR_PTR));

	// Set FifoPtrAddr to FifoTxPtrBase to read written data
	spi_write_register(fd, FIFO_ADDR_PTR, spi_read_register(fd, FIFO_TX_BASE_ADDR));
	printf("[After setting] FIFO_ADDR_PTR: 0x%02X\n", spi_read_register(fd, FIFO_ADDR_PTR));

	for(uint8_t i = 0x00; i < spi_read_register(fd, PAYLOAD_LENGTH); i++) {
		printf("[%d] Reading from FIFO_ADDR_PTR: 0x%02X, FIFO data: 0x%02X \n", i, spi_read_register(fd, FIFO_ADDR_PTR), spi_read_register(fd, FIFO));
	}
	spi_write_register(fd, 0x09, 0x8F);
	// Set TX mode to initiate data transmission
	// CAUTION: DO NOT initiate transmission unless antenna is attached to LoRa
	spi_write_register(fd, OP_MODE, LORA_TX);
	// TODO wait for some time? maybe check the TxDone interrupt?
	//while(spi_read_register(fd, IRQ_FLAGS) != 0x08) {}
	printf("Press ENTER to cancel TX...\n");
	getchar();
	//spi_write_register(fd, IRQ_FLAGS, 0x08);

	// Put LoRa in Sleep mode
	printf("Setting LORA_SLEEP...");
	spi_write_register(fd, OP_MODE, 0x80);
	while(spi_read_register(fd, OP_MODE) != 0x80) {}
	printf(" LORA_SLEEP set [OP_MODE: 0x%02X]\n", spi_read_register(fd, OP_MODE));

	close(fd);
	return 0;
}
