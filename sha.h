#ifndef SHA_H
#define SHA_H

#include "lora_frame.h"
#include "log_info.h"


/*
 * sha_create
 *
 * Oblicza SHA-256 podanej ramki:
 * - liczone dla wszystkich pól ramki, oprócz SHA256, które jest pomijane
 * - dane w .data są wypełnione zerami do MAX_DATA_SIZE
 *
 * Parametry:
 * frame: ramka LoRa
 * output: bufor wyjściowy (32 bajty), do którego ma zostać zapisana obliczona
 * suma kontrolna
 *
 * Nie zwraca żadnej wartości
 */
void sha_create(lora_frame *frame, void *output_buffer);

/*
 * sha_verify
 *
 * Sprawdza, czy obliczona suma SHA-256 jest taka sama jak ta otrzymana
 * w ramce.
 *
 * Parametry:
 * frame: wczytana ramka LoRa
 *
 * Wartości zwracane:
 * 0 = ok, SHA się zgadza
 * 1 = są różne
 */
int sha_verify(lora_frame *frame);

#endif
