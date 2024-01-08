#ifndef SHA_H
#define SHA_H

#include "lora_frame.h"
#include "log_info.h"


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
