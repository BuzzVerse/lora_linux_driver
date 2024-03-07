// Register addresses
#define FIFO			0x00
#define OP_MODE			0x01
#define FIFO_ADDR_PTR		0x0D
#define FIFO_TX_BASE_ADDR	0x0E
#define FIFO_RX_BASE_ADDR	0x0F
#define FIFO_RX_CURRENT_ADDR	0x10
#define IRQ_FLAGS		0x12
#define FIFO_RX_BYTES_NB	0x13
#define PAYLOAD_LENGTH		0x22

// Register values
#define FSK_OOK_SLEEP		0x08
#define FSK_OOK_STANDBY		0x09	// default mode
#define LORA_SLEEP		0x88
#define LORA_STANDBY		0x89
#define LORA_TX			0x8B	// only enable if antenna is attached
#define LORA_RX_CONT		0x8D
#define LORA_RX_SINGLE		0x8E
