// Register addresses
#define FIFO			        0x00
#define OP_MODE			        0x01
#define FR_MSB			        0x06
#define FR_MID			        0x07
#define FR_LSB			        0x08
#define PA_CONFIG		        0x09
#define LNA			            0x0C
#define FIFO_ADDR_PTR		    0x0D
#define RX_CONFIG		        0x0D
#define FIFO_TX_BASE_ADDR	    0x0E
#define RSSI_CONFIG		        0x0E
#define FIFO_RX_BASE_ADDR	    0x0F
#define FIFO_RX_CURRENT_ADDR	0x10
#define IRQ_FLAGS		        0x12
#define FIFO_RX_BYTES_NB	    0x13
#define MODEM_STATUS            0x18
#define AFC_FEI			        0x1A
#define MODEM_CONFIG_1		    0x1D
#define MODEM_CONFIG_2		    0x1E
#define PREAMBLE_DETECT		    0x1F
#define PREAMBLE_LENGTH_MSB     0x20
#define PREAMBLE_LENGTH_LSB     0x21
#define PAYLOAD_LENGTH		    0x22
#define MAX_PAYLOAD_LENGTH	    0x23
#define OSC 			        0x24
#define MODEM_CONFIG_3		    0x26
#define SYNC_CONFIG		        0x27
#define SYNC_VALUE_1		    0x28
#define SYNC_VALUE_2		    0x29
#define SYNC_VALUE_3		    0x2A
#define PACKET_CONFIG_1		    0x30
#define FIFO_THRESH		        0x35
#define DETECT_OPTIMIZE 	    0x31
#define DETECTION_THRESHOLD	    0x37
#define IMAGE_CAL		        0x3B
#define DIO_MAPPING_1		    0x40
#define DIO_MAPPING_2		    0x41

// Register values
#define FSK_OOK_SLEEP		    0x08
#define FSK_OOK_STANDBY		    0x09	// default mode
/*
#define LORA_SLEEP		        0x88
#define LORA_STANDBY		    0x89
#define LORA_TX			        0x8B	// only enable if antenna is attached
#define LORA_RX_CONT		    0x8D
#define LORA_RX_SINGLE		    0x8E
*/
// LowFreqModeOff
// TODO rename the constants so it is clear whether LowFreqMode is active or not
#define LORA_SLEEP		        0x80
#define LORA_STANDBY		    0x81
#define LORA_TX			        0x83	// only enable if antenna is attached
#define LORA_RX_CONT		    0x85
#define LORA_RX_SINGLE		    0x86

// ?
#define IMAGE_CAL_MASK		    0xBF
#define IMAGE_CAL_START		    0x40
#define IMAGE_CAL_RUNNING	    0x20
#define FREQ_STEP		        61.03515625
