#ifndef LORA_READ_H
#define LORA_READ_H

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


#endif
