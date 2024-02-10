/* wysyłanie danych przez SPI */
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

static uint8_t mode = 0;
static uint8_t bits = 8;
static uint32_t speed = 500000;
static uint16_t delay = 0;

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(*a))

void print_manual()
{
    printf("Usage: test_spi_write [SPI device]\n");
    printf("Example: test_spi_write /dev/spidev1.1\n");
}

int spi_ioctl_init(int fd){
    if(ioctl(fd, SPI_IOC_WR_MODE, &mode) == -1){
        printf("SPI_IOC_WR_MODE: error\n");
        return -1;
    }

    if(ioctl(fd, SPI_IOC_RD_MODE, &mode) == -1)
    {
        printf("SPI_IOC_RD_MODE: error\n");
        return -1;
    }

    if(ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) == -1)
    {
        printf("SPI_IOC_WR_BITS_PER_WORD: error\n");
        return -1;
    }

    if(ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits) == -1)
    {
        printf("SPI_IOC_RD_BITS_PER_WORD: error\n");
        return -1;
    }

    if(ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1)
    {
        printf("SPI_IOC_WR_MAX_SPEED_HZ: error\n");
        return -1;
    }

    if(ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) == -1)
    {
        printf("SPI_IOC_RD_MAX_SPEED_HZ: error\n");
        return -1;
    }
    return 0;
}

/* spi_send/spi_get: wysyła/odbiera wiadomość, fd to urządzenie SPI
zwraca 0 if ok, 1 if błąd */

int spi_send(int fd, uint8_t* tx, int size)
{
    // struct from spidev.h, describes a single SPI transfer
    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long) tx,
        .rx_buf = 0,
        .len = size,
        .delay_usecs = delay,
        .speed_hz = speed,
        .bits_per_word = bits,
    };

    if(ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 1) return 1;
    return 0;
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        print_manual();
        return 0;
    }

    // "/dev/spidevx.y" is always 15 characters long
    // (including the string termination character '\0')
    char device[15];
    strcpy(device, argv[1]);

    int ret = 0;
    int fd = open(device, O_RDWR);
    if(fd < 0)
    {
        printf("Device %s couldn't be opened.\n", device);
        return -1;
    }
    spi_ioctl_init(fd);

    for(;;){
        uint8_t tx[] = "czesc xd";
        printf("send: %d\n", spi_send(fd, tx, ARRAY_SIZE(tx)));
        sleep(1);
    }
    close(fd);
    return 0;
}
