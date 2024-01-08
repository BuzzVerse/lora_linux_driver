
/*
 * Czeka na ramki i zapisuje je do plików w podanym katalogu
 * Każde uruchomienie programu NADPISUJE POPRZEDNIE RAMKI
 * bo liczy numery od nowa
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include "log_info.h"

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


