#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "lora_registers.h"
#include "spi_io.h"

int main()
{
	int fd = open("/dev/spidev1.0", O_RDWR);
    	if (fd == 0) {
        	perror("Can't open device. Check permissions and if file exists");
        	return 1;
    	}
	// TODO Reset the chip - must be done outside this program, by setting the RST pin on the chip

	// RX chain calibration
	// Save initial values:
	uint8_t pa_config_init_val = spi_read_register(fd, PA_CONFIG);
	uint32_t initialFreq = ( double )( ( ( uint32_t )spi_read_register(fd, FR_MSB) << 16 ) |
                              ( ( uint32_t )spi_read_register(fd, FR_MID ) << 8 ) |
                              ( ( uint32_t )spi_read_register(fd, FR_LSB) ) ) * ( double )FREQ_STEP;

	spi_write_register(fd, PA_CONFIG, 0x00);
	while(spi_read_register(fd, PA_CONFIG) != 0x00) {}

	// RX chain calibration for LF band:
	spi_write_register(fd, IMAGE_CAL, 
			(spi_read_register(fd, IMAGE_CAL) & IMAGE_CAL_MASK) | IMAGE_CAL_START);
	while((spi_read_register(fd, IMAGE_CAL) & IMAGE_CAL_RUNNING) == IMAGE_CAL_RUNNING) {}

	// RX chain calibration for HF band:
	uint32_t freq = 868000000;
	freq = (uint32_t)((double)freq/(double)FREQ_STEP);
	spi_write_register(fd, FR_MSB, (uint8_t)( (freq >> 16) & 0xFF) );
	spi_write_register(fd, FR_MID, (uint8_t)( (freq >> 8) & 0xFF) );
	spi_write_register(fd, FR_LSB, (uint8_t)( freq & 0xFF) );
	spi_write_register(fd, IMAGE_CAL, 
			(spi_read_register(fd, IMAGE_CAL) & IMAGE_CAL_MASK) | IMAGE_CAL_START);
	while((spi_read_register(fd, IMAGE_CAL) & IMAGE_CAL_RUNNING) == IMAGE_CAL_RUNNING) {}

	// Restore initial values
	spi_write_register(fd, PA_CONFIG, pa_config_init_val);
	initialFreq = (uint32_t)((double)initialFreq/(double)FREQ_STEP);
	spi_write_register(fd, FR_MSB, (uint8_t)( (initialFreq >> 16) & 0xFF) );
	spi_write_register(fd, FR_MID, (uint8_t)( (initialFreq >> 8) & 0xFF) );
	spi_write_register(fd, FR_LSB, (uint8_t)( initialFreq & 0xFF) );



	// Set sleep mode
	spi_write_register(fd, OP_MODE, FSK_OOK_SLEEP);

	// Set radio setting
	spi_write_register(fd, LNA, 0x23);
	spi_write_register(fd, RX_CONFIG, 0x1E);
	spi_write_register(fd, RSSI_CONFIG, 0xD2);
	spi_write_register(fd, AFC_FEI, 0x01);
	spi_write_register(fd, PREAMBLE_DETECT, 0xAA);
	spi_write_register(fd, OSC, 0x07);
	spi_write_register(fd, SYNC_CONFIG, 0x12);
	spi_write_register(fd, SYNC_VALUE_1, 0xC1);
	spi_write_register(fd, SYNC_VALUE_2, 0x94);
	spi_write_register(fd, SYNC_VALUE_3, 0xC1);
	spi_write_register(fd, PACKET_CONFIG_1, 0xD8);
	spi_write_register(fd, FIFO_THRESH, 0x8F);
	spi_write_register(fd, IMAGE_CAL, 0x02);
	spi_write_register(fd, DIO_MAPPING_1, 0x00);
	spi_write_register(fd, DIO_MAPPING_2, 0x30);
	spi_write_register(fd, MAX_PAYLOAD_LENGTH, 0x40);
	
	// Set modem (set LoRa mode)
	//spi_write_register(fd, OP_MODE, LORA_SLEEP);
	


	// To enable communication, LoRa has to be set in LoRa Standby mode
	// Default mode is FSK/OOK Standby
	// FSK/OOK and LoRa modes can only be switched in Sleep mode, therefore the order of operations is as follows:
	// 1) Set LoRa Sleep mode
	// 2) Set LoRa Standby mode
	
	// Set LowFreqMode off? and remember to keep it set
	//spi_write_register(fd, OP_MODE, 0x01);

	// 1) Set LoRa Sleep mode
	printf("Setting LORA_SLEEP...");
	spi_write_register(fd, OP_MODE, LORA_SLEEP);
	while(spi_read_register(fd, OP_MODE) != LORA_SLEEP) {}
	printf(" LORA_SLEEP set [OP_MODE = 0x%02X]\n", spi_read_register(fd, OP_MODE));

	
	// Set MODEM_CONFIG_3 to 0x04
	spi_write_register(fd, MODEM_CONFIG_3, 0x04);
	
	// Set PA_CONFIG to 0x8F
	spi_write_register(fd, PA_CONFIG, 0x8F);

    	// 2) Set LoRa Standby mode
	printf("Setting LORA_STANDBY...");
	spi_write_register(fd, OP_MODE, LORA_STANDBY);
	while(spi_read_register(fd, OP_MODE) != LORA_STANDBY) {}
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
	// The if statement inside the loop prints the value of IRQ_FLAGS if it changes
	// i is a very primitive way to exit the while loop after a couple IRQ change detections
	int i = 16;
	uint8_t irq_value = 0x00;
	//while((spi_read_register(fd, IRQ_FLAGS) & 0x40)!= 0x40) {}
	while(i > 0) { if(spi_read_register(fd, IRQ_FLAGS) != irq_value) {i--; irq_value = spi_read_register(fd, IRQ_FLAGS); printf("[Inside while loop] IRQ_FLAGS: 0x%02X\n", irq_value);} }
	//printf("Press ENTER to cancel RX...\n");
	//getchar();
	printf("IRQ_FLAGS: 0x%02X \n", spi_read_register(fd, IRQ_FLAGS));

	// write 1 to clear the interrupt
	spi_write_register(fd, IRQ_FLAGS, 0x40);
	// Set LoRa Standby mode
	printf("Setting LORA_STANDBY...");
	spi_write_register(fd, OP_MODE, LORA_STANDBY);
	while(spi_read_register(fd, OP_MODE) != LORA_STANDBY) {}
	printf(" LORA_STANDBY set [OP_MODE = 0x%02X]\n", spi_read_register(fd, OP_MODE));

	// Ensure that ValidHeader, PayloadCrcError, RxDone and RxTimeout interrputs in the status register RegIrqFlags are not asserted to ensure that packet reception has terminated successfully (i.e. no flags should be set).
	// In case of errors: skip the following steps and discard the packet
	printf("IRQ_FLAGS: 0x%02X \n", spi_read_register(fd, IRQ_FLAGS));

	// Read received payload from the FIFO - datasheet page 41
	// 1) Set RegFifoAddrPtr to RegFifoRxCurrentAddr = set the FIFO pointer to the location of the last packet received in the FIFO
	// Currently reads the whole RX FIFO (set AddrPtr to RxBaseAddr)
	spi_write_register(fd, FIFO_ADDR_PTR, spi_read_register(fd, FIFO_RX_BASE_ADDR));
	while(spi_read_register(fd, FIFO_ADDR_PTR) != spi_read_register(fd, FIFO_RX_BASE_ADDR)) {}
	// 2) Read the register RegFifo, RegRxNbBytes times
	for(uint8_t i = 0x00; i < 0xFF; i++) {
		printf("[%d] Reading from FIFO_ADDR_PTR: 0x%02X, FIFO data: 0x%02X \n", i, spi_read_register(fd, FIFO_ADDR_PTR), spi_read_register(fd, FIFO));
	}
	
	// Set LoRa Sleep mode
	printf("Setting LORA_SLEEP...");
	spi_write_register(fd, OP_MODE, LORA_SLEEP);
	while(spi_read_register(fd, OP_MODE) != LORA_SLEEP) {}
	printf(" LORA_SLEEP set [OP_MODE = 0x%02X]\n", spi_read_register(fd, OP_MODE));

	close(fd);
	return 0;
}
