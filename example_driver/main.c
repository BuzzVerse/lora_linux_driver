/*
 * Reads data from all registers on LoRa and writes them to a file.
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define SPI_SPEED_HZ 500000
#define SPI_WRITE 0x80
#define SPI_READ  0x00
#define OUT_FILE "./out_file.txt"

/**
 * Function to write data to register
 * @param fd - file descriptor
 * @param reg - register address
 * @param value - value to write
 */
void spi_write_register(int fd, uint8_t reg, uint8_t value) {
    uint8_t tx[] = {reg | SPI_WRITE, value};
    uint8_t rx[2] = {0, 0};
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = 2,
        .speed_hz = SPI_SPEED_HZ,
        .bits_per_word = 8,
    };

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 0) {
        perror("SPI Write Error");
    }
}

/**
 * Function to read data from register
 * @param fd - file descriptor
 * @param reg - register address
 * @return - value from register
 */
uint8_t spi_read_register(int fd, uint8_t reg) {
    uint8_t tx[] = {reg | SPI_READ, 0x00}; // We are sending register address and 0x00 to push out data from slave
    uint8_t rx[2] = {0, 0};
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)tx,
        .rx_buf = (unsigned long)rx,
        .len = 2,
        .speed_hz = SPI_SPEED_HZ,
        .bits_per_word = 8,
    };

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 0) {
        perror("SPI Read Error");
        return 0;
    }

    return rx[1]; // Return second byte from rx buffer that is read value from register
}

int main() {
    // Prepare an array to store values after reading them from LoRa,
    // but before saving them to a file.
    // NOTE: max. 255 values can be stored and
    // if there's less than 255 values the rest of the array
    // is filled with 0s
    uint8_t values[255];
    for(int i = 0; i < 255; i++) {values[i] = 0x00;}
    
    int fd = open("/dev/spidev0.0", O_RDWR);
    if (fd == 0) {
        perror("Can't open device. Check permissions and if file exists");
        return 1;
    }

    int i = 0;
    // Read registers from 0x00 to 0x70
    for (uint8_t reg = 0x00; reg <= 0x70; reg++) {
        uint8_t value = spi_read_register(fd, reg);
	//uint8_t value = reg;
        printf("Register at 0x%02X has value: 0x%02X\n", reg, value);
	values[i] = value;
	i++;
    }

    close(fd);

    // Open file to write output to
    // NOTE: writes HEX values
    FILE *fp;
    fp = fopen("out_file.txt", "wb+");
    for(int i = 0; i < 255; i++) {
	fprintf(fp, "0x%02X ", values[i]);	
    } 
    fclose(fp);

    return 0;
}
