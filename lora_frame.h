#include <stdint.h>

typedef struct lora_frame {
    uint8_t MAC[6];
    uint16_t packet_type;
    uint8_t packet_number;
    uint8_t packet_count;
    uint8_t data_count;
    uint8_t data[212];
    uint8_t SHA256[32];
} __attribute__((packed)) lora_frame;

