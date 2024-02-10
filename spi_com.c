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
	printf("Usage: spice.c [SPI device]\n");
	printf("Example: spice.c /dev/spidev1.1\n");
}

static void transfer(int fd, uint8_t* tx, int size)
{
	int ret;
	uint8_t *rx;
	rx = (uint8_t*)calloc(size, sizeof(uint8_t));

	// struct from spidev.h, describes a single SPI transfer
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = size,
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if(ret < 1)
		printf("Couldn't send SPI message.\n");

	printf("Message received:\n");
	for(ret = 0; ret < size; ret++) {
		printf("%.2X ", rx[ret]);
	}
	printf("\n");
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



	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if(ret == -1)
	{
		printf("SPI_IOC_WR_MODE: error\n");
		return -1;
	}

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if(ret == -1)
	{
		printf("SPI_IOC_RD_MODE: error\n");
		return -1;
	}

	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if(ret == -1)
	{
		printf("SPI_IOC_WR_BITS_PER_WORD: error\n");
		return -1;
	}

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if(ret == -1)
	{
		printf("SPI_IOC_RD_BITS_PER_WORD: error\n");
		return -1;
	}

	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if(ret == -1)
	{
		printf("SPI_IOC_WR_MAX_SPEED_HZ: error\n");
		return -1;
	}

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if(ret == -1)
	{
		printf("SPI_IOC_RD_MAX_SPEED_HZ: error\n");
		return -1;
	}

	// hard-coded message, if it appears after executing the program
	// it means the SPI device works
	uint8_t tx[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
	transfer(fd, tx, ARRAY_SIZE(tx));

	close(fd);
	printf("Device %s closed.\n", device);

	return 0;
}
