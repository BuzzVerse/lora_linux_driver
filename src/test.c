#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "colors.h"

void lora_reset(void) {
    for (int i = 0x0; i < 0x2; i++) {
        FILE *fptr66, *fptr69;
        fptr66 = fopen("/sys/class/gpio/gpio66/value", "w");
        fptr69 = fopen("/sys/class/gpio/gpio69/value", "w");

        if ((fptr66 == NULL) || (fptr69 == NULL)) {
            printf("%s[Error]%s Reading GPIO failed.\n", C_RED, C_DEFAULT);
            return; 
        }

        fprintf(fptr66, "%d", i);
        fprintf(fptr69, "%d", i);

        sleep((double)(0.0001));

        fclose(fptr66);
        fclose(fptr69);
    }

    sleep((double)(0.005));

    printf("%s[RESET]%s Ok\n", C_GREEN, C_DEFAULT);
}

int main(void) {
    lora_reset();
}
