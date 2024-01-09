#ifndef LORA_READ_H
#define LORA_READ_H

#include "lora_frame.h"

/*
 * read_byte(int fd, int timeout)
 * odczytuje następny bajt z pliku o podanym deskryptorze fd
 * fd musi być prawidłowym deskryptorem pliku
 *
 * timeout określa czas w milisekundach ile maksymalnie będzie czekać
 * timeout -1 oznacza, że czeka w nieskończoność
 *
 *  zwraca:
 *  0-255:  bajt odczytany z pliku
 *  -1:     timeout
 *  -2:     error
 *  -3:     eof, koniec pliku
 */
int read_byte(int fd, int timeout);

int read_frame(lora_frame* frame, int LORA_TIMEOUT, int spi_fd, char* DEV_SPI, int MESSAGE_SIZE);

int write_frame(lora_frame* frame, int out_fd, char* out_name);

#endif