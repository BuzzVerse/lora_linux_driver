// Register addresses
#define FIFO			0x00
#define OP_MODE			0x01
#define FR_MSB			0x06
#define FR_MID			0x07
#define FR_LSB			0x08
#define PA_CONFIG		0x09
#define LNA			0x0C
#define FIFO_ADDR_PTR		0x0D
#define FIFO_TX_BASE_ADDR	0x0E
#define FIFO_RX_BASE_ADDR	0x0F
#define FIFO_RX_CURRENT_ADDR	0x10
#define IRQ_FLAGS		0x12
#define FIFO_RX_BYTES_NB	0x13
#define MODEM_CONFIG_1		0x1D
#define MODEM_CONFIG_2		0x1E
#define PAYLOAD_LENGTH		0x22
#define MODEM_CONFIG_3		0x26
#define DETECT_OPTIMIZE 	0x31
#define DETECTION_THRESHOLD	0x37

// Register values
#define FSK_OOK_SLEEP		0x08
#define FSK_OOK_STANDBY		0x09	// default mode
#define LORA_SLEEP		0x88
#define LORA_STANDBY		0x89
#define LORA_TX			0x8B	// only enable if antenna is attached
#define LORA_RX_CONT		0x8D
#define LORA_RX_SINGLE		0x8E
