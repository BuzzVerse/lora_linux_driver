#ifndef LORA_FRAME_H
#define LORA_FRAME_H

#include <stdint.h>

/* MAX_DATA_SIZE: ile bajtów danych wchodzi do jednego pakietu
 *
 * na wypadek, gdyby się coś zmieniło i mielibyśmy obsługiwać pakiety
 * z większą ilością danych na jeden pakiet:
 */
#define MAX_DATA_SIZE   212

typedef struct lora_frame {
    uint8_t MAC[6];
    uint16_t packet_type;
    uint8_t packet_number;
    uint8_t packet_count;
    uint8_t data_count;
    uint8_t data[MAX_DATA_SIZE];
    uint8_t SHA256[32];
} __attribute__((packed)) lora_frame;


#endif
