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

	// To enable communication, LoRa has to be set in LoRa Standby mode
	// Default mode is FSK/OOK Standby
	// FSK/OOK and LoRa modes can only be switched in Sleep mode, therefore the order of operations is as follows:
	// 1) Set LoRa Sleep mode
	// 2) Set LoRa Standby mode

	// 1) Set LoRa Sleep mode
	spi_write_register(fd, REG_OP_MODE, LORA_SLEEP);

    	// 2) Set LoRa Standby mode
	spi_write_register(fd, REG_OP_MODE, LORA_STANDBY);

	// Set FifoAddrPtr to FifoRxBaseAddr
	//spi_write_register(fd, FIFO_ADDR_PTR, FIFO_RX_BASE_ADDR);

	// Mode request: RX Continuous to initiate receive operation
	//spi_write_register(fd, REG_OP_MODE, LORA_RX_CONT);

	// Radio automatically returns to Standby mode

	// Read received payload from the FIFO
	
	close(fd);

	return 0;
}
