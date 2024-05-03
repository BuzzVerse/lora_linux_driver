#ifndef LORA_UTILITY_H
#define LORA_UTILITY_H

void lora_initialize(int fd);

void print_modem_status(int fd);

void print_irq_flags(int fd);

#endif
