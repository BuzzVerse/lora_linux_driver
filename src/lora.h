#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <string.h>
#include <time.h>

#include "api/driver_api.h"
#include "driver/lora_driver.h"
#include "colors.h"

#define MESSAGE_SIZE    10
#define LOGFILE         "/var/log/lora.log"

static int fd;

int spidev_open(const char* dev);

void spidev_close();

void print_buffer(uint8_t* buf, uint8_t len);

void buffer_to_string(uint8_t* buffer, char* destination);

int loginfo(const char* msg);

lora_status_t lora_receive(uint8_t* data);

lora_status_t temp_init(void);
