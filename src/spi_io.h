#ifndef SPI_IO_H
#define SPI_IO_H

#include <stdint.h>

#define SPI_SPEED_HZ	500000
#define SPI_WRITE	0x80
#define SPI_READ	0x00

void spi_write_register(int fd, uint8_t reg, uint8_t value);

uint8_t spi_read_register(int fd, uint8_t reg);

#endif

