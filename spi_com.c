/*
*  Simple SPIDEV communication
*/
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
	printf("Usage: spi_com [SPI device]\n");
	printf("Example: spi_com /dev/spidev1.1\n");
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

int spi_get(int fd, uint8_t* out_buffer, int size)
{
	// struct from spidev.h, describes a single SPI transfer
	struct spi_ioc_transfer tr = {
		.tx_buf = 0,
		.rx_buf = (unsigned long)out_buffer,
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
	printf("Device %s opened.\n", device);



	// hard-coded message, if it appears after executing the program
	// it means the SPI device works
	uint8_t tx[] = "czesc xd";
	uint8_t rx[ARRAY_SIZE(tx)];
	rx[0] = 0;
	printf("send: %d\n", spi_send(fd, tx, ARRAY_SIZE(tx)));
	printf("get: %d\n", spi_get(fd, rx, ARRAY_SIZE(rx)));
	printf("otrzymano: %s\n", rx);

	for(int i = 0; i < ARRAY_SIZE(rx); i++) {
		printf("%02X ", rx[i]);
	}
	printf("\n");

	printf("write: %d\n", write(fd, "12345", 5));
	char xd[16] = {0};
	printf("read: %d\n", read(fd, xd, 15));

	printf("xd: %s\n", xd);

	close(fd);
	printf("Device %s closed.\n", device);

	return 0;
}
