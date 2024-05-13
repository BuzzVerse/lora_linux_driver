#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "colors.h"

void lora_reset(void) {
    system("sudo sh -c 'echo 0 > /sys/class/gpio/gpio66/value'");
    system("sudo sh -c 'echo 0 > /sys/class/gpio/gpio69/value'");
    system("sleep .0001s");
    system("sudo sh -c 'echo 1 > /sys/class/gpio/gpio66/value'");
    system("sudo sh -c 'echo 1 > /sys/class/gpio/gpio69/value'");
    system("sleep .005s");
}

int main(void) {
    lora_reset();
}
