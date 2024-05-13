#include <fcntl.h>
#include <stdio.h>
// #include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "spi_io.h"
#include "lora_registers.h"
#include "lora_utility.h"

int main()
{
    //spi_init();

    int fd = open("/dev/spidev0.0", O_RDWR);
    if (fd == 0) {
        perror("Can't open device. Check permissions and if file exists");
        return 1;
    }

	// Reset the chip
    //lora_reset();

	// Set LoRa Sleep mode
	printf("Setting LORA_SLEEP...");
	spi_write_register(fd, OP_MODE, LORA_SLEEP);
	while(spi_read_register(fd, OP_MODE) != LORA_SLEEP) {}
	printf(" LORA_SLEEP set [OP_MODE: 0x%02X]\n", spi_read_register(fd, OP_MODE));

    lora_initialize(fd);

    // Set LoRa Standby mode
	printf("Setting LORA_STANDBY...");
	spi_write_register(fd, OP_MODE, LORA_STANDBY); 
	while(spi_read_register(fd, OP_MODE) != LORA_STANDBY) {}
	printf(" LORA_STANDBY set [OP_MODE: 0x%02X]\n", spi_read_register(fd, OP_MODE));

	// Fill the FIFO with data to transmit:
	// 1) Set FifoPtrAddr to FifoTxPtrBase
	spi_write_register(fd, FIFO_ADDR_PTR, spi_read_register(fd, FIFO_TX_BASE_ADDR));

	spi_write_register(fd, PAYLOAD_LENGTH, 0x06); // 6 bytes for testing purposes
	printf("PAYLOAD_LENGTH: 0x%02X \n", spi_read_register(fd, PAYLOAD_LENGTH));

	// 2) Write PAYLOAD_LENGTH bytes to the FIFO
	for(uint8_t i = 0x00; i < spi_read_register(fd, PAYLOAD_LENGTH); i++) {
		spi_write_register(fd, FIFO, i + 16);
	}

	// Set FifoPtrAddr to FifoTxPtrBase to read written data
	spi_write_register(fd, FIFO_ADDR_PTR, spi_read_register(fd, FIFO_TX_BASE_ADDR));

    printf("Data to send:\n");
	for(uint8_t i = 0x00; i < spi_read_register(fd, PAYLOAD_LENGTH); i++) {
		printf("[%d] FIFO address: 0x%02X, data: 0x%02X \n", i, spi_read_register(fd, FIFO_ADDR_PTR), spi_read_register(fd, FIFO));
	}

	// Set TX mode to initiate data transmission
	// CAUTION: DO NOT initiate transmission unless antenna is attached to LoRa
	spi_write_register(fd, OP_MODE, LORA_TX);

	// Wait until TxDone interrupt is set
	while((spi_read_register(fd, IRQ_FLAGS) & 0x08) != 0x08) {}

	printf("Packet sent successfully - IRQ_FLAGS: 0x%02X\n", spi_read_register(fd, IRQ_FLAGS));
	// Write 1 to clear the interrupt
    spi_write_register(fd, IRQ_FLAGS, 0xFF);

	// Put LoRa in Sleep mode
	printf("Setting LORA_SLEEP...");
	spi_write_register(fd, OP_MODE, LORA_SLEEP);
	while(spi_read_register(fd, OP_MODE) != LORA_SLEEP) {}
	printf(" LORA_SLEEP set [OP_MODE: 0x%02X]\n", spi_read_register(fd, OP_MODE));

	close(fd);
	return 0;
}
