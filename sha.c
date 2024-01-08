#include "sha.h"

/* zdefiniowane w sha256.c */
void sha256(const void *data, uint64_t len, void *output);


int sha_verify(lora_frame *frame) {
    uint8_t calculated_sha[32];

    log_info(WARN, "sha_verify: NIEPRZETESTOWANE");

    /* SHA jest obliczane dla wszystkich pól ramki za wyjątkiem
     * samego SHA
     */
    sha256(frame, sizeof(frame) - sizeof(frame->SHA256), &calculated_sha);

    for(int i = 0; i < 32; i++) {
        if (frame->SHA256[i] != calculated_sha[i])
            return 1;
    }
    return 0;

}
