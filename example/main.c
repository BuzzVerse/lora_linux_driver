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

/**
 * Function to initialize SPI device.
 * 
 * Note: At the end of the program, you should call deinit function to close SPI device.
 * 
 * @return - fd - file descriptor for SPI device. If initialization fails, 0 is returned.
 */
int init(const char* spi_device_file_path) {
    
    int fd;
    int mode = SPI_MODE_0;
    int bits = 8; // Bits per word
    int speed = SPI_SPEED_HZ;

    // Open SPI device
    fd = open("/dev/spidev0.0", O_RDWR);
    if (fd < 0) {
        perror("Can't open SPI device. Do you have permission?");
        exit(1);
    }

    // Set SPI mode
    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0) {
        perror("Can't set SPI mode");
        close(fd);
        return 0;
    }

    // Set bits per word
    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) {
        perror("Can't set bits per word");
        close(fd);
        return 0;
    }

    // Set max speed
    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
        perror("Can't set max speed for SPI");
        close(fd);
        return 0;
    }

    printf("SPI device was successfully initialized\n");
    return fd;
}
/**
 * Function to close SPI device. Should be called at the end of the program,
 * to release resources.
 * 
 * @param fd - file descriptor
 */
void deinit(int fd) {
    close(fd);
    printf("SPI device was successfully closed\n");
}


int main() {

    int fd = init("/dev/spidev0.0");

    if (fd == 0) {
        perror("SPI initialization failed");
        return 1;
    }

    // Read registers from 0x00 to 0x70
    for (uint8_t reg = 0x00; reg <= 0x70; reg++) {
        uint8_t value = spi_read_register(fd, reg);
        printf("Register at 0x%02X has value: 0x%02X\n", reg, value);
    }

    deinit(fd);

    return 0;
}
