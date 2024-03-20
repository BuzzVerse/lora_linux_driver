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
	int fd = open("/dev/spidev1.0", O_RDWR);
    	if (fd == 0) {
        	perror("Can't open device. Check permissions and if file exists");
        	return 1;
    	}

	// To enable communication, LoRa has to be set in LoRa Standby mode
	// Default mode is FSK/OOK Standby
	// FSK/OOK and LoRa modes can only be switched in Sleep mode, therefore the order of operations is as follows:
	// 1) Set LoRa Sleep mode
	// 2) Set LoRa Standby mode
	
	// Set LowFreqMode off? and remember to keep it set
	spi_write_register(fd, OP_MODE, 0x01);

	// 1) Set LoRa Sleep mode
	printf("Setting LORA_SLEEP...");
	//spi_write_register(fd, OP_MODE, LORA_SLEEP);
	spi_write_register(fd, OP_MODE, 0x80); // LORA_SLEEP, LowFreqMode off
	while(spi_read_register(fd, OP_MODE) != 0x80) {}
	printf(" LORA_SLEEP set [OP_MODE = 0x%02X]\n", spi_read_register(fd, OP_MODE));

	// Set RegLna to 0x23
	spi_write_register(fd, LNA, 0x23);
	
	// Set MODEM_CONFIG_3 to 0x04
	spi_write_register(fd, MODEM_CONFIG_3, 0x04);
	
	// Set PA_CONFIG to 0x8F
	spi_write_register(fd, PA_CONFIG, 0x8F);

    	// 2) Set LoRa Standby mode
	printf("Setting LORA_STANDBY...");
	//spi_write_register(fd, OP_MODE, LORA_STANDBY);
	spi_write_register(fd, OP_MODE, 0x81); // LORA_STANDBY, LowFreqMode off
	while(spi_read_register(fd, OP_MODE) != 0x81) {}
	printf(" LORA_STANDBY set [OP_MODE = 0x%02X]\n", spi_read_register(fd, OP_MODE));

	// Set FR_MSB to 0x6C (default)
	spi_write_register(fd, FR_MSB, 0x6C);
	
	// Set FR_MID to 0x40
	spi_write_register(fd, FR_MID, 0x40);

	// Set FR_LSB to 0x00 (default)
	spi_write_register(fd, FR_LSB, 0x00);

	// Set MODEM_CONFIG_2 to 0x74
	spi_write_register(fd, MODEM_CONFIG_2, 0x74);
	
	// Set MODEM_CONFIG_1 to 0x72
	spi_write_register(fd, MODEM_CONFIG_1, 0x72);

	// Set DETECT_OPTIMIZE to 0xC3
	spi_write_register(fd, DETECT_OPTIMIZE, 0xC3);

	// Set DETECTION_THRESHOLD to 0x0A
	spi_write_register(fd, DETECTION_THRESHOLD, 0x0A);

	// Set MODEM_CONFIG_2 to 0x74 again?
	spi_write_register(fd, MODEM_CONFIG_2, 0x74);

	// Set FifoAddrPtr to FifoRxBaseAddr
	spi_write_register(fd, FIFO_ADDR_PTR, FIFO_RX_BASE_ADDR);

	printf("IRQ_FLAGS: 0x%02X \n", spi_read_register(fd, IRQ_FLAGS));
	// Mode request: RX Continuous to initiate receive operation
	printf("Initiating RX...");
	spi_write_register(fd, OP_MODE, LORA_RX_CONT);
	printf(" RX_CONT set [OP_MODE = 0x%02X]\n", spi_read_register(fd, OP_MODE));
	printf("IRQ_FLAGS: 0x%02X \n", spi_read_register(fd, IRQ_FLAGS));

	// Wait until RX_DONE interrupt is set
	//while(spi_read_register(fd, IRQ_FLAGS) != 0x40) {}
	printf("Press ENTER to cancel RX...\n");
	getchar();
	printf("IRQ_FLAGS: 0x%02X \n", spi_read_register(fd, IRQ_FLAGS));

	// write 1 to clear the interrupt
	spi_write_register(fd, IRQ_FLAGS, 0x40);
	// Set LoRa Standby mode
	printf("Setting LORA_STANDBY...");
	spi_write_register(fd, OP_MODE, 0x81);
	while(spi_read_register(fd, OP_MODE) != 0x81) {}
	printf(" LORA_STANDBY set [OP_MODE = 0x%02X]\n", spi_read_register(fd, OP_MODE));

	// Ensure that ValidHeader, PayloadCrcError, RxDone and RxTimeout interrputs in the status register RegIrqFlags are not asserted to ensure that packet reception has terminated successfully (i.e. no flags should be set).
	// In case of errors: skip the following steps and discard the packet
	printf("IRQ_FLAGS: 0x%02X \n", spi_read_register(fd, IRQ_FLAGS));

	// Read received payload from the FIFO - datasheet page 41
	// 1) Set RegFifoAddrPtr to RegFifoRxCurrentAddr = set the FIFO pointer to the location of the last packet received in the FIFO
	spi_write_register(fd, FIFO_ADDR_PTR, spi_read_register(fd, FIFO_RX_BASE_ADDR));
	while(spi_read_register(fd, FIFO_ADDR_PTR) != spi_read_register(fd, FIFO_RX_BASE_ADDR)) {}
	// 2) Read the register RegFifo, RegRxNbBytes times
	for(uint8_t i = 0x00; i < 0x80; i++) {
		printf("[%d] Reading from FIFO_ADDR_PTR: 0x%02X, FIFO data: 0x%02X \n", i, spi_read_register(fd, FIFO_ADDR_PTR), spi_read_register(fd, FIFO));
	}
	
	// Set LoRa Sleep mode
	printf("Setting LORA_SLEEP...");
	spi_write_register(fd, OP_MODE, 0x80);
	while(spi_read_register(fd, OP_MODE) != 0x80) {}
	printf(" LORA_SLEEP set [OP_MODE = 0x%02X]\n", spi_read_register(fd, OP_MODE));

	close(fd);
	return 0;
}
