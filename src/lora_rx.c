#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#include "lora_registers.h"
#include "spi_io.h"

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

int main()
{
	int fd = open("/dev/spidev1.0", O_RDWR);
    	if (fd == 0) {
        	perror("Can't open device. Check permissions and if file exists");
        	return 1;
    	}

	// TODO Reset the chip	

    /*
	// RX chain calibration ----------------------------------------------------
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
    // -------------------------------------------------------------------------



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
	*/


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

    spi_write_register(fd, FIFO_RX_BASE_ADDR, 0x00);    
	spi_write_register(fd, LNA, spi_read_register(fd, LNA) | 0x03);
	spi_write_register(fd, MODEM_CONFIG_3, 0x04);
    spi_write_register(fd, PA_CONFIG, 0x8F);
    
    /*
	spi_write_register(fd, MODEM_CONFIG_3, 0x04);
	spi_write_register(fd, PA_CONFIG, 0x8F);
    spi_write_register(fd, FR_MSB, 0x6C); // default: 0x6C
	spi_write_register(fd, FR_MID, 0x40);
	spi_write_register(fd, FR_LSB, 0x00); // default: 0x00
	//spi_write_register(fd, MODEM_CONFIG_2, 0x74);
	//spi_write_register(fd, MODEM_CONFIG_1, 0x72);
	spi_write_register(fd, DETECT_OPTIMIZE, 0xC3);
	spi_write_register(fd, DETECTION_THRESHOLD, 0x0A);
	spi_write_register(fd, MODEM_CONFIG_2, 0xC0);
	spi_write_register(fd, MODEM_CONFIG_1, 0x48);
    spi_write_register(fd, PREAMBLE_LENGTH_LSB, 0x00);
    */

    // 2) Set LoRa Standby mode
	printf("Setting LORA_STANDBY...");
	spi_write_register(fd, OP_MODE, LORA_STANDBY);
	while(spi_read_register(fd, OP_MODE) != LORA_STANDBY) {}
	printf(" LORA_STANDBY set [OP_MODE = 0x%02X]\n", spi_read_register(fd, OP_MODE));

    spi_write_register(fd, MODEM_CONFIG_1, 0x48); // BW = 4, CR = 4/8
    spi_write_register(fd, MODEM_CONFIG_2, 0xC4); // SF = 12, CRC enabled

	//spi_write_register(fd, FR_MSB, 0x6C); // default: 0x6C
	//spi_write_register(fd, FR_MID, 0x40);
	//spi_write_register(fd, FR_LSB, 0x00); // default: 0x00
	//spi_write_register(fd, MODEM_CONFIG_2, 0x74);
	//spi_write_register(fd, MODEM_CONFIG_2, 0xC0);
	//spi_write_register(fd, MODEM_CONFIG_1, 0x72);
	//spi_write_register(fd, MODEM_CONFIG_1, 0x48);
	//spi_write_register(fd, DETECT_OPTIMIZE, 0xC3);
	//spi_write_register(fd, DETECTION_THRESHOLD, 0x0A);
	// Set MODEM_CONFIG_2 to 0x74 again?
	//spi_write_register(fd, MODEM_CONFIG_2, 0x74);



	// Set FifoAddrPtr to FifoRxBaseAddr
	spi_write_register(fd, FIFO_ADDR_PTR, FIFO_RX_BASE_ADDR);

	printf("IRQ_FLAGS: 0x%02X \n", spi_read_register(fd, IRQ_FLAGS));
	// Mode request: RX Continuous to initiate receive operation
	printf("Initiating RX...");
	spi_write_register(fd, OP_MODE, LORA_RX_CONT);
	sleep((double)(0.1));
	printf(" RX_CONT set [OP_MODE = 0x%02X]\n", spi_read_register(fd, OP_MODE));
	printf("IRQ_FLAGS: 0x%02X \n", spi_read_register(fd, IRQ_FLAGS));


	// Wait until RX_DONE interrupt is set
	// The if statement inside the loop prints the value of IRQ_FLAGS if it changes
	// i is a very primitive way to exit the while loop after a couple IRQ change detections
	int i = 64;
	uint8_t irq_value = 0x00;
    uint8_t modem_status = 0x10;
	//while((spi_read_register(fd, IRQ_FLAGS) & 0x40)!= 0x40) {}
	while(i > 0) {
        if(spi_read_register(fd, IRQ_FLAGS) == 0x50) { // 0x05 = RxDone interrupt
		    break;
        }
        if(spi_read_register(fd, IRQ_FLAGS) != irq_value) {
            i--;
            irq_value = spi_read_register(fd, IRQ_FLAGS);
            printf("[Inside loop] IRQ_FLAGS: 0x%02X\n", irq_value);
        }
        if(spi_read_register(fd, MODEM_STATUS) != modem_status) {
            modem_status = spi_read_register(fd, MODEM_STATUS);
            print_modem_status(fd); 
        }

    }

    // Set LoRa Standby mode
	printf("Setting LORA_STANDBY...");
	spi_write_register(fd, OP_MODE, LORA_STANDBY);
	//while(spi_read_register(fd, OP_MODE) != LORA_STANDBY) {}
	sleep((double)(0.1));
	printf(" LORA_STANDBY set [OP_MODE = 0x%02X]\n", spi_read_register(fd, OP_MODE));

	printf("IRQ_FLAGS: 0x%02X \n", spi_read_register(fd, IRQ_FLAGS));
    print_irq_flags(fd);
    print_modem_status(fd); 

	// write 1 to clear the interrupt
	spi_write_register(fd, IRQ_FLAGS, 0x40);
	
	// Ensure that ValidHeader, PayloadCrcError, RxDone and RxTimeout interrputs in the status register RegIrqFlags are not asserted to ensure that packet reception has terminated successfully (i.e. no flags should be set).
	// In case of errors: skip the following steps and discard the packet
	//printf("IRQ_FLAGS: 0x%02X \n", spi_read_register(fd, IRQ_FLAGS));



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
	//while(spi_read_register(fd, OP_MODE) != LORA_SLEEP) {}
	sleep((double)(0.1));
	printf(" LORA_SLEEP set [OP_MODE = 0x%02X]\n", spi_read_register(fd, OP_MODE));

	close(fd);
	return 0;
}
