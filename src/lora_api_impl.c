#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h> 

#include "../include/api/driver_api.h"
#include "lora_registers.h"
#include "spi_io.h"
#include "colors.h"

static int fd;

api_status_t spi_init(void) {
    // Enable SPI
    // For SPI0, /dev/spidev0.#
    system("config-pin p9_17 spi_cs");
    system("config-pin p9_18 spi");
    system("config-pin p9_21 spi");
    system("config-pin p9_22 spi_sclk");

    // For SPI1, /dev/spidev1.#
    system("config-pin p9_28 spi_cs");
    system("config-pin p9_29 spi");
    system("config-pin p9_30 spi");
    system("config-pin p9_31 spi_sclk");

    // TODO Enable GPIO pins like in gpio_enable.sh OR maybe in a separate function?
    // gpio66_direction=/sys/class/gpio/gpio66/direction
    // gpio66_value=/sys/class/gpio/gpio66/value
    // gpio69_direction=/sys/class/gpio/gpio69/direction
    // gpio69_value=/sys/class/gpio/gpio69/value
    FILE *gpio66d, *gpio66v, *gpio69d, *gpio69v;
    gpio66d = fopen("/sys/class/gpio/gpio66/direction", "w");
    gpio66v = fopen("/sys/class/gpio/gpio66/value", "w");
    gpio69d = fopen("/sys/class/gpio/gpio69/direction", "w");
    gpio69v = fopen("/sys/class/gpio/gpio69/value", "w");

    fseek(gpio66d, 0, SEEK_SET);
    fseek(gpio69d, 0, SEEK_SET);
    fseek(gpio66v, 0, SEEK_SET);
    fseek(gpio69v, 0, SEEK_SET);

    fflush(gpio66d);
    fprintf(gpio66d, "out");
    fflush(gpio66v);
    fprintf(gpio66v, "1");
    fflush(gpio69d);
    fprintf(gpio69d, "out");
    fflush(gpio69v);
    fprintf(gpio69v, "1");

    fclose(gpio66d);
    fclose(gpio66v);
    fclose(gpio69d);
    fclose(gpio69v);
    

    // echo out > $gpio66_direction
    // echo 1 > $gpio66_value
    // echo out > $gpio69_direction
    // echo 1 > $gpio69_value

    fd = open("/dev/spidev1.0", O_RDWR);
    printf("Opened fd: %d\n", fd);

    printf("%s[LORA]%s Init\n", C_GREEN, C_DEFAULT);

    return API_OK;
}

// read a register and save it to a variable
api_status_t spi_read(uint8_t reg, uint8_t* val) {
    uint8_t tx[] = {reg | SPI_READ, 0x00};
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
        return API_SPI_ERROR;
    }

    *val = rx[1];
    return API_OK;
}

// TODO not sure if it works??
api_status_t spi_read_buf(uint8_t reg, uint8_t* buf, uint16_t len) {
    for(uint16_t i = 0; i < len; i++) {
        if(spi_read(reg, buf) == API_SPI_ERROR) {
            return API_SPI_ERROR;
        }
        buf++;
    }

    return API_OK;
}

api_status_t spi_write(uint8_t reg, uint8_t val) {
    uint8_t tx[] = {reg | SPI_WRITE, val};
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
        return API_SPI_ERROR;
    }

    return API_OK;
}

api_status_t spi_write_buf(uint8_t reg, uint8_t* buf, uint16_t len) {
    for(uint16_t i = 0; i < len; i++) {
        if(spi_write(reg, *buf) == API_SPI_ERROR) {
            return API_SPI_ERROR;
        }
        buf++;
    }
    return API_OK;
}

void lora_delay(uint32_t ticks) {
    sleep(ticks);    
}

void lora_reset(void) {
    FILE *fptr66, *fptr69;
    fptr66 = fopen("/sys/class/gpio/gpio66/value", "w");
    fptr69 = fopen("/sys/class/gpio/gpio69/value", "w");

    if ((fptr66 == NULL) || (fptr69 == NULL)) {
        printf("%s[Error]%s Reading GPIO failed.\n", C_RED, C_DEFAULT);
        return; 
    }

    fseek(fptr66, 0, SEEK_SET);
    fseek(fptr69, 0, SEEK_SET);

    for (int i = 0; i < 2; i++) {
        fprintf(fptr66, "%d", i);
        fflush(fptr66);

        fprintf(fptr69, "%d", i);
        fflush(fptr69);

        sleep(1);
    }
    printf("%s[RESET]%s Ok\n", C_GREEN, C_DEFAULT);
}

void spi_close() {
    //printf("Closing fd: %d\n", fd);
    close(fd);
}
