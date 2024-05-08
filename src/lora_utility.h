#ifndef LORA_UTILITY_H
#define LORA_UTILITY_H

#include <stdint.h>

void lora_initialize(int fd);

void lora_dump_registers(int fd);

void set_mode(int fd, uint8_t mode);

void set_bandwidth(int fd, int bw);

void set_coding_rate(int fd, int cr);

void set_spreading_factor(int fd, int sf);

void enable_crc(int fd);

void print_modem_status(int fd);

void print_irq_flags(int fd);

void lora_reset(void);

#endif
