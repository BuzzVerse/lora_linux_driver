#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <string.h>
#include <time.h>

#include "colors.h"
#include "api/driver_api.h"
#include "driver/lora_driver.h"
#include "packet/packet.h"

#define LOGFILE         "/var/log/lora.log"

static int fd;

int spidev_open(const char* dev);

void spidev_close();

void print_buffer(uint8_t* buf, uint8_t len);

void buffer_to_string(uint8_t* buffer, size_t buffer_size, char* destination, size_t destination_size);

int loginfo(const char* msg);

lora_status_t lora_receive(packet_t* packet);

lora_status_t temp_init(void);

void pack_packet(uint8_t *buffer, packet_t *packet, size_t data_size);

size_t get_data_size(DataType type);
