
/*
 * Czeka na ramki i zapisuje je do plików w podanym katalogu
 * Każde uruchomienie programu NADPISUJE POPRZEDNIE RAMKI
 * bo liczy numery od nowa
 */


#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include "log_info.h"
#include "lora_frame.h"

int read_byte(int fd, int timeout) {
    unsigned char byte_buf;
    int poll_result;
    ssize_t read_result;
    struct pollfd p; /* strukturka dla poll() */
    p.fd = fd;
    p.events = POLLIN;

start_poll:
    poll_result = poll(&p, 1, timeout);
    if(poll_result == 0) return -1; /* timeout */
    if(poll_result == -1) return -2; /* error */
    if(p.revents & POLLERR || p.revents & POLLNVAL) return -2; /* error */

    /* 
     * POLLHUP oznacza że jest "peer closed connection" i może oznaczać
     * że jest EOF
     * chyba że jednocześnie jest POLLIN, to znaczy że jeszcze coś zostało
     * do przeczytania
     */
    if (p.revents & POLLHUP && !(p.revents & POLLIN)) return -3; /* EOF */

    /* poll się udał, tylko nie wiadomo czy zwrócił POLLIN */
    if ( !(p.revents & POLLIN) ) {
        /* POLLIN=0, nie ma danych do czytania, jeszcze raz poll() */
        goto start_poll;
    }

    /* są dane do odczytania
     * read może zwrócić 1 jeżeli odczyta pomyślnie 1 bajt
     * albo -1 jeżeli error
     * w tym przypadku skoro poll() powiedział że są dane, nie powinno
     * być sytuacji że read zwraca 0 bo jest EOF, więc każdą wartość != 1
     * traktuję jako błąd
     */
    read_result = read(fd, &byte_buf, 1);
    if (read_result == 1) return byte_buf;
    if (read_result == 0) return -3; /* EOF */
    /* else: error w read() */
    return -1;
}

int read_frame(lora_frame* frame, int LORA_TIMEOUT, int spi_fd, const char* DEV_SPI) {
    size_t i;

    log_info(INFO, "Oczekiwanie na %s ...", DEV_SPI);

    size_t header_size = sizeof(frame->MAC) + sizeof(frame->packet_type) + sizeof(frame->packet_number) + sizeof(frame->packet_count) + sizeof(frame->data_count);

    /* odczyt nagłówka (wszystkiego aż do danych = 11 bajtów) */
    /* 1. bajt nagłówka */
    int x = read_byte(spi_fd, -1);
    if (x == -3) {
        /* EOF, koniec pliku, więc nie ma sensu kontynuować */
        log_info(ERROR, "Napotkano koniec pliku '%s', koniec.", DEV_SPI);
        // TODO zrobić ładną obsługe błedu w main()
        exit(1);
    }
    if (x < 0) {
        log_info(XERROR, "read_byte: Błąd odczytu '%s'", DEV_SPI);
        return -1;
    }
    ((uint8_t*)frame)[0] = x;
    
    log_info(INFO, "Wczytano 1. bajt");
    

    /* reszta nagłówka */
    for(i = 1; i < header_size; i++) {
        x = read_byte(spi_fd, LORA_TIMEOUT);
        
        if (x == -1) {
            /* timeout */
            log_info(XERROR, "read_byte(): TIMEOUT - "
                    "Nie otrzymano kolejnego bajtu ramki");
            log_info(INFO, "LORA_TIMEOUT = %d", LORA_TIMEOUT);
            /*log_info(INFO, "Nr ramki: " FORMAT_MSG_NUMBER, msg_number);*/
            log_info(INFO, "Otrzymano %zu z %zu B", i, (size_t)sizeof(lora_frame));
            break;
        }
        else if (x == -2) {
            /* błąd odczytu z DEV_SPI */
            log_info(XERROR, "read_byte(): Błąd odczytu '%s'", DEV_SPI);
            break;
        }
        else if (x == -3) {
            /* koniec pliku DEV_SPI */
            log_info(ERROR, "Napotkano koniec pliku '%s'", DEV_SPI);
            return 1;
        }
        ((uint8_t*)frame)[i] = x;
    }

    log_info(INFO, "Wczytano nagłówek");

    log_info(INFO, "data_count = %u", (unsigned) frame->data_count);

    /* odczyt data biorąc pod uwagę data_count */
    for(i = header_size; i < header_size + frame->data_count; i++) {
        x = read_byte(spi_fd, LORA_TIMEOUT);

        if (x == -1) {
            /* timeout */
            log_info(XERROR, "read_byte(): TIMEOUT - "
                    "Nie otrzymano kolejnego bajtu ramki");
            log_info(INFO, "LORA_TIMEOUT = %d", LORA_TIMEOUT);
            /*log_info(INFO, "Nr ramki: " FORMAT_MSG_NUMBER, msg_number);*/
            log_info(INFO, "Otrzymano %zu z %zu B", i, (size_t)sizeof(lora_frame));
            break;
        }
        else if (x == -2) {
            /* błąd odczytu z DEV_SPI */
            log_info(XERROR, "read_byte(): Błąd odczytu '%s'", DEV_SPI);
            break;
        }
        else if (x == -3) {
            /* koniec pliku DEV_SPI */
            log_info(ERROR, "Napotkano koniec pliku '%s'", DEV_SPI);
            return 1;
        }
        ((uint8_t*)frame)[i] = x;
    }
    /*
    log_info(INFO, "Wczytano data");
    */

    /* odczyt SHA */
    for(i = header_size + sizeof(frame->data); i < sizeof(*frame); i++) {
        x = read_byte(spi_fd, LORA_TIMEOUT);
        
        if (x == -1) {
            /* timeout */
            log_info(XERROR, "read_byte(): TIMEOUT - "
                    "Nie otrzymano kolejnego bajtu ramki");
            log_info(INFO, "LORA_TIMEOUT = %d", LORA_TIMEOUT);
            /*log_info(INFO, "Nr ramki: " FORMAT_MSG_NUMBER, msg_number);*/
            log_info(INFO, "Otrzymano %zu z %zu B", i, (size_t)sizeof(lora_frame));
            break;
        }
        else if (x == -2) {
            /* błąd odczytu z DEV_SPI */
            log_info(XERROR, "read_byte(): Błąd odczytu '%s'", DEV_SPI);
            break;
        }
        else if (x == -3) {
            /* koniec pliku DEV_SPI */
            log_info(ERROR, "Napotkano koniec pliku '%s'", DEV_SPI);
            close(spi_fd);
            return 1;
        }

        ((uint8_t*)frame)[i] = x;
    }

    if (i == sizeof(lora_frame)) {
            /* nie przerwano for(), a więc nie było błędu I/O,
             * można wyświetlić info że zapisano i jest ok
             */
            log_info(INFO, "Zapisano ramkę LoRa do zmiennej");
        }

    return 0;
}

int write_frame(lora_frame* frame, int out_fd, const char* out_name) {
    size_t i;

    for(i = 0; i < sizeof(frame); i++) {
        if(write(out_fd, &frame[i], 1) == -1) {
            log_info(ERROR, "write_frame(): błąd zapisu");
        }
    }

    log_info(INFO, "Zapisano ramkę LoRa do pliku '%s'", out_name);

    return 0;
}

