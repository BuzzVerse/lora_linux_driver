#include "sha.h"
#include "sha256.h"


void hexdumpf(const char *name, const void *var, size_t size)
{
    printf("HEX: %s\n", name);
    printf("        00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F\n");
    printf("        -- -- -- -- -- -- -- --  -- -- -- -- -- -- -- --");

    for(size_t i = 0; i < size; i++) {
        if (i % 0x10 == 0)
            printf("\n  %04lX  ", (i/16)<<4);
        else if ( i % 8 == 0)
            printf("  ");
        else
            printf(" ");

        printf("%02X", ((unsigned char*)var)[i]);
    }
    printf("\n\n");
    fflush(stdout);
}


/* hexdump(zmienna, rozmiar)
 * wyświetla zmienną w widoku HEX
 */
#define hexdump(VAR,SIZE) hexdumpf(#VAR,VAR,SIZE)





void sha_create(lora_frame *frame, void *output_buffer) {
    sha256(frame, sizeof(frame) - sizeof(frame->SHA256), output_buffer);
}

int sha_verify(lora_frame *frame) {
    uint8_t calculated_sha[32];

    log_info(WARN, "sha_verify: NIEPRZETESTOWANE");

    /* SHA jest obliczane dla wszystkich pól ramki za wyjątkiem
     * samego SHA
     */

    sha_create(frame, &calculated_sha);

    hexdump(frame,sizeof(*frame));

    log_debug("SHA otrzymane: %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X ",
            frame->SHA256[0],
            frame->SHA256[1],
            frame->SHA256[2],
            frame->SHA256[3],
            frame->SHA256[4],
            frame->SHA256[5],
            frame->SHA256[6],
            frame->SHA256[7],
            frame->SHA256[8],
            frame->SHA256[9],
            frame->SHA256[10],
            frame->SHA256[11],
            frame->SHA256[12],
            frame->SHA256[13],
            frame->SHA256[14],
            frame->SHA256[15],
            frame->SHA256[16],
            frame->SHA256[17],
            frame->SHA256[18],
            frame->SHA256[19],
            frame->SHA256[20],
            frame->SHA256[21],
            frame->SHA256[22],
            frame->SHA256[23],
            frame->SHA256[24],
            frame->SHA256[25],
            frame->SHA256[26],
            frame->SHA256[27],
            frame->SHA256[28],
            frame->SHA256[29],
            frame->SHA256[30],
            frame->SHA256[31] );

    log_debug("SHA obliczone: %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X ",
            calculated_sha[0],
            calculated_sha[1],
            calculated_sha[2],
            calculated_sha[3],
            calculated_sha[4],
            calculated_sha[5],
            calculated_sha[6],
            calculated_sha[7],
            calculated_sha[8],
            calculated_sha[9],
            calculated_sha[10],
            calculated_sha[11],
            calculated_sha[12],
            calculated_sha[13],
            calculated_sha[14],
            calculated_sha[15],
            calculated_sha[16],
            calculated_sha[17],
            calculated_sha[18],
            calculated_sha[19],
            calculated_sha[20],
            calculated_sha[21],
            calculated_sha[22],
            calculated_sha[23],
            calculated_sha[24],
            calculated_sha[25],
            calculated_sha[26],
            calculated_sha[27],
            calculated_sha[28],
            calculated_sha[29],
            calculated_sha[30],
            calculated_sha[31] );


    for(int i = 0; i < 32; i++) {
        if (frame->SHA256[i] != calculated_sha[i])
            return 1;
    }
    return 0;

}
