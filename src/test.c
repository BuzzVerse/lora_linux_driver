#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "colors.h"

void lora_reset(void) {
    FILE *fptr66, *fptr69;
    fptr66 = fopen("/sys/class/gpio/gpio66/value", "w");
    fptr69 = fopen("/sys/class/gpio/gpio69/value", "w");

    if ((fptr66 == NULL) || (fptr69 == NULL)) {
        printf("%s[Error]%s Reading GPIO failed.\n", C_RED, C_DEFAULT);
        return; 
    }

    fseek(fptr66, 0, SEEK_SET);
    fseek(fptr69, 0, SEEK_SET);

    fprintf(fptr66, "%d", 0);
    fflush(fptr66);

    fprintf(fptr69, "%d", 0);
    fflush(fptr69);

    sleep(1);

    fprintf(fptr66, "%d", 1);
    fflush(fptr66);

    fprintf(fptr69, "%d", 1);
    fflush(fptr69);

    fclose(fptr66);
    fclose(fptr69);

    sleep(1);

    printf("%s[RESET]%s Ok\n", C_GREEN, C_DEFAULT);
}

int main(void) {
    lora_reset();
}
