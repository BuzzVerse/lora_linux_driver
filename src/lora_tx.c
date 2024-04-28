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
	int fd = open("/dev/spidev0.0", O_RDWR);
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

	printf("[Before setting] OP_MODE: 0x%02X\n", spi_read_register(fd, OP_MODE));
	
	//spi_write_register(fd, OP_MODE, 0x01);

	// Set LoRa Sleep mode
	printf("Setting LORA_SLEEP...");
	spi_write_register(fd, OP_MODE, LORA_SLEEP);
	//spi_write_register(fd, OP_MODE, 0x80);
	// there is a small delay between calling spi_write_register()
	// and the register value being actually written, 
	// hence the need to wait for a while
	while(spi_read_register(fd, OP_MODE) != LORA_SLEEP) {}
	printf(" LORA_SLEEP set [OP_MODE: 0x%02X]\n", spi_read_register(fd, OP_MODE));

	//spi_write_register(fd, LNA, 0x23);
	spi_write_register(fd, MODEM_CONFIG_3, 0x04);
	spi_write_register(fd, PA_CONFIG, 0x8F);
	spi_write_register(fd, FR_LSB, 0x6C);
	spi_write_register(fd, FR_MID, 0x40);
	spi_write_register(fd, FR_MSB, 0x00);
	//spi_write_register(fd, MODEM_CONFIG_2, 0x74);
	//spi_write_register(fd, MODEM_CONFIG_1, 0x72);
	spi_write_register(fd, DETECT_OPTIMIZE, 0xC3);
	spi_write_register(fd, DETECTION_THRESHOLD, 0x0A);
	spi_write_register(fd, MODEM_CONFIG_2, 0xC0);
	spi_write_register(fd, MODEM_CONFIG_1, 0x48);

	// Set LoRa Standby mode
	printf("Setting LORA_STANDBY...");
	spi_write_register(fd, OP_MODE, LORA_STANDBY); 
	while(spi_read_register(fd, OP_MODE) != LORA_STANDBY) {}
	printf(" LORA_STANDBY set [OP_MODE: 0x%02X]\n", spi_read_register(fd, OP_MODE));

	// Fill the FIFO with data to transmit:
	// 1) Set FifoPtrAddr to FifoTxPtrBase
	spi_write_register(fd, FIFO_ADDR_PTR, spi_read_register(fd, FIFO_TX_BASE_ADDR));
	printf("[After setting] FIFO_ADDR_PTR: 0x%02X\n", spi_read_register(fd, FIFO_ADDR_PTR));

	spi_write_register(fd, PAYLOAD_LENGTH, 0x08);
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
	//spi_write_register(fd, PA_CONFIG, 0x8F);
	// Set TX mode to initiate data transmission
	// CAUTION: DO NOT initiate transmission unless antenna is attached to LoRa
	spi_write_register(fd, OP_MODE, LORA_TX);
	printf("Before loop - IRQ_FLAGS: 0x%02X\n", spi_read_register(fd, IRQ_FLAGS));
	// Wait until TxDone interrupt is set
	while((spi_read_register(fd, IRQ_FLAGS) & 0x08) != 0x08) {
		//printf("Inside loop - IRQ_FLAGS: 0x%02X\n", spi_read_register(fd, IRQ_FLAGS));
	}
	printf("Packet sent successfully - IRQ_FLAGS: 0x%02X\n", spi_read_register(fd, IRQ_FLAGS));
	spi_write_register(fd, IRQ_FLAGS, 0x08);

	// Put LoRa in Sleep mode
	printf("Setting LORA_SLEEP...");
	spi_write_register(fd, OP_MODE, LORA_SLEEP);
	while(spi_read_register(fd, OP_MODE) != LORA_SLEEP) {}
	printf(" LORA_SLEEP set [OP_MODE: 0x%02X]\n", spi_read_register(fd, OP_MODE));

	close(fd);
	return 0;
}
