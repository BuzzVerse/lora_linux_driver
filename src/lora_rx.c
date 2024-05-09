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
    int fd = open("/dev/spidev1.0", O_RDWR);
    if (fd == 0) {
        perror("Can't open device. Check permissions and if file exists");
        return 1;
    }

	// Reset the chip	
    lora_reset();

	// To enable communication, LoRa has to be set in LoRa Standby mode
	// Default mode is FSK/OOK Standby
	// FSK/OOK and LoRa modes can only be switched in Sleep mode, therefore the order of operations is as follows:
	// 1) Set LoRa Sleep mode
	// 2) Set LoRa Standby mode

	// 1) Set LoRa Sleep mode
	printf("Setting LORA_SLEEP...");
	spi_write_register(fd, OP_MODE, LORA_SLEEP);
	while(spi_read_register(fd, OP_MODE) != LORA_SLEEP) {}
	printf(" LORA_SLEEP set [OP_MODE = 0x%02X]\n", spi_read_register(fd, OP_MODE));

    lora_initialize(fd);

    lora_dump_registers(fd);

    // 2) Set LoRa Standby mode
	printf("Setting LORA_STANDBY...");
	spi_write_register(fd, OP_MODE, LORA_STANDBY);
	while(spi_read_register(fd, OP_MODE) != LORA_STANDBY) {}
	printf(" LORA_STANDBY set [OP_MODE = 0x%02X]\n", spi_read_register(fd, OP_MODE));

	// Set FifoAddrPtr to FifoRxBaseAddr
	spi_write_register(fd, FIFO_ADDR_PTR, FIFO_RX_BASE_ADDR);

	// Mode request: RX Continuous to initiate receive operation
	printf("Initiating RX...");
	spi_write_register(fd, OP_MODE, LORA_RX_CONT);
	while(spi_read_register(fd, OP_MODE) != LORA_RX_CONT) {}
	printf(" RX_CONT set [OP_MODE = 0x%02X]\n", spi_read_register(fd, OP_MODE));

	uint8_t irq_value = 0x00;
    uint8_t modem_status = 0x10;
	while(1) {
        if((spi_read_register(fd, IRQ_FLAGS) & 0x50) == 0x50) { // wait until ValidHeader and RxDone interrupts are set
		    break;
        }
        if(spi_read_register(fd, IRQ_FLAGS) != irq_value) { // read IRQ_FLAGS if it changes
            irq_value = spi_read_register(fd, IRQ_FLAGS);
            //printf("[Inside loop] IRQ_FLAGS: 0x%02X\n", irq_value);
        }
        if(spi_read_register(fd, MODEM_STATUS) != modem_status) { // read MODEM_STATUS if it changes
            modem_status = spi_read_register(fd, MODEM_STATUS);
            //print_modem_status(fd); 
        }
    }

    // Set LoRa Standby mode
	printf("Setting LORA_STANDBY...");
	spi_write_register(fd, OP_MODE, LORA_STANDBY);
	while(spi_read_register(fd, OP_MODE) != LORA_STANDBY) {}
	//sleep((double)(0.1));
	printf(" LORA_STANDBY set [OP_MODE = 0x%02X]\n", spi_read_register(fd, OP_MODE));

    // Ensure that ValidHeader, PayloadCrcError, RxDone and RxTimeout interrputs in the status register RegIrqFlags are not asserted to ensure that packet reception has terminated successfully (i.e. no flags should be set).
	// In case of errors: skip the following steps and discard the packet

	printf("IRQ_FLAGS: 0x%02X \n", spi_read_register(fd, IRQ_FLAGS));
    print_irq_flags(fd);
    print_modem_status(fd); 

	// write 1 to clear the interrupt
	spi_write_register(fd, IRQ_FLAGS, 0xFF);



	// Read received payload from the FIFO - datasheet page 41
	// 1) Set the FIFO pointer to the location of the last packet received in the FIFO
	spi_write_register(fd, FIFO_ADDR_PTR, spi_read_register(fd, FIFO_RX_CURRENT_ADDR));
	// 2) Read the register RegFifo, RegRxNbBytes times
    uint8_t nb_bytes = spi_read_register(fd, FIFO_RX_BYTES_NB);
    uint8_t fifo_addr_ptr;
    uint8_t fifo_data;
	printf("NB_BYTES: 0x%02X\n", nb_bytes);
	for(uint8_t i = 0x00; i < nb_bytes; i++) {
        fifo_addr_ptr = spi_read_register(fd, FIFO_ADDR_PTR);
        fifo_data = spi_read_register(fd, FIFO);
		printf("[%d] Reading from FIFO_ADDR_PTR: 0x%02X, FIFO data: 0x%02X \n", i, fifo_addr_ptr, fifo_data);
	}
	


	// Set LoRa Sleep mode
	printf("Setting LORA_SLEEP...");
	spi_write_register(fd, OP_MODE, LORA_SLEEP);
	while(spi_read_register(fd, OP_MODE) != LORA_SLEEP) {}
	//sleep((double)(0.1));
	printf(" LORA_SLEEP set [OP_MODE = 0x%02X]\n", spi_read_register(fd, OP_MODE));

	close(fd);
	return 0;
}
