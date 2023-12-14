
/*
 * Czeka na ramki i zapisuje je do plików w podanym katalogu
 * Każde uruchomienie programu NADPISUJE POPRZEDNIE RAMKI
 * bo liczy numery od nowa
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <poll.h>

#include "log_info.h"

/* MESSAGE_SIZE
 * rozmiar ramki-pakietu, który ma być naraz odczytany z LoRa
 */
#define MESSAGE_SIZE    64 /* test */
/*
 * wydaje mi się że fajnie by było, gdyby na późniejszym etapie
 * był jakiś jeden wspólny plik nagłówkowy, który by definiował
 * strukturkę, jak wygląda zawartość ramki przesyłanej LoRą.
 * Wtedy zamiast ustawiać rozmiar ramki na sztywno, można by
 * wziąć sizeof( typ_danych_strukturki_LoRa )
 * ?
 */


/* DEV_SPI
 * nazwa urządzenia w /dev/
 * to będzie coś w stylu /dev/spidevX.Y
 */
#define DEV_SPI         "/dev/urandom" /* test */
/* OUT_DIR
 * nazwa katalogu wyjściowego, do którego będą zapisywane otrzymane
 * ramki
 * na razie nie mam pojęcia co się z nimi będzie działo, więc po prostu
 * będą wrzucane do kolejnych plików
 * MUSI BYĆ ZAKOŃCZONY '/'
 */
#define OUT_DIR         "./ramki/"

/* LORA_TIMEOUT
 * czas oczekiwania na kolejne bajty ramki, w milisekundach
 * (pomiędzy ramkami czeka oczywiście w nieskończoność)
 */
#define LORA_TIMEOUT    1000 /* test */

/* FORMAT_MSG_NUMBER
 * Format wyświetlania numeru ramki, dla funkcji typu printf
 * W przypadku zmiany typu danych zmiennej msg_number trzeba go
 * odpowiednio zmienić:
 *
 * PRIu32 to format okreslający uint32_t
 * https://en.cppreference.com/w/c/types/integer
 */
#define FORMAT_MSG_NUMBER   "%"PRIu32

/* OUT_FILE_MODE
 * uprawnienia do pliku wyjściowego, które są przekazywane jako "mode"
 * tzn. trzeci parametr do open() przy tworzeniu pliku wyjściowego
 *
 * user:group jest brane (chyba) wg konta na którym działa program
 *
 * numery jak w chmod
 * to znaczy, bardziej poprawnie sie to robi stałymi
 * (są wymienione w man 2 open)
 * ale nazwy w stylu S_IRWXU, S_IRWXG na pewno nie zostały stworzone
 * z myślą o tym że ktoś to kiedykolwiek będzie czytał
 *
 * to 0 z przodu musi być, bo numer jest zapisany ósemkowo
 *
 * 6 oznacza RW-
 * 0666 daje RW dla usera, RW dla group i RW dla others
 */
#define OUT_FILE_MODE   0666


/*
 * TODO na później
 * - nazwa pliku, do którego zapisuje się ramkę mogła by zawierać datę
 *   i godzinę kiedy ta ramka została otrzymana -- o ile na BeagleBone mamy
 *   dostępny jakiś zegar?
 *   bo teraz przy każdym uruchomieniu programu będą sie nadpisywać poprzednie
 *   ramki
 *   ewentualnie zrobić jakiś plik w którym będzie się zapisywać numer ostatnio
 *   zapisanej ramki i od tego będzie zaczynało nabijać kolejne
 *
 * - podczas odczytywania ramki można by dać jakiś TIMEOUT, żeby read()
 *   nie czekał w nieskończoność na brakujące bajty pakietu jeżeli nie
 *   przychodzą w jakimś sensownym czasie, powiedzmy kilku sekund
 *   (kilkunastu? w sumie nie wiem jak szybki transfer tu będzie)
 *
 * - ładne zakończenie programu po otrzymaniu sygnału SIGINT/SIGTERM/...?
 *   close(), free() itd.
 */


/* zmienne są globalne, żeby po otrzymaniu sygnału zakończenia
 * programu można było mieć dostęp do deskryptorów otwartych plików
 * i wskaźników na zaalokowaną pamięć
 */
int spi_fd = -1;
int out_fd = -1;
char *out_name = NULL;



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
 *  -3:     EOF, koniec pliku
 */
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
        puts("goto poll");
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


int main(void) {
    /*
     * numer ramki: msg_number
     * ten arm cortex-a8 co jest a beaglebonie jest 32-bitowy, więc
     * dla niego naturalną liczbą będzie 32-bitowa
     * raczej nie będziemy mieć 2^32 ramek żeby to się przepełniło
     * i była potrzeba dla uint64?
     *
     * w przypadku zmiany rozmiaru zmiennej trzeba zmienić też:
     *
     * 1) obliczenia rozmiaru bufora nazwy poniżej
     * bo nie chce mi sie teraz pisać algorytmu który wyliczy
     * ile cyfr może zająć liczba jeżeli mamy na wejściu sizeof()
     * zmiennej w bajtach
     *
     * 2) definicję FORMAT_MSG_NUMBER
     */
    uint32_t msg_number = 1;

    /* rozmiar bufora nazwy:
     * długość OUT_DIR + nazwa właściwa + 1 NUL na końcu
     *
     * właściwa nazwa to jest kolejny numer ramki msg_number
     * zmienna jest 32-bitowa, więc max wartość to:
     * 2^32 - 1 = 4 294 967 295
     * co zajmuje 10 cyfr
     *
     * stąd: rozmiar = strlen(OUT_DIR) + 10 + 1
     */
    const size_t digit_count = 10;
    size_t out_dir_length = strlen(OUT_DIR);
    size_t out_path_size = out_dir_length + digit_count + 1;

    out_name = malloc(out_path_size);
    if (!out_name) {
        log_info(ERROR, "Błąd alokacji pamięci");
        return 1;
    }

    log_info(0, "LoRa: %s", DEV_SPI);
    log_info(0, "Katalog wyjściowy: %s", OUT_DIR);
    log_info(0, "Rozmiar ramki: %u", (unsigned int) MESSAGE_SIZE);
    log_info(0, "Czas oczekiwania na jeden bajt (timeout): %d ms",
            LORA_TIMEOUT);


    /* początek nazwy - ścieżka OUT_DIR jest stała więc można by ją
     * wpisać od razu, a potem w pętli będzie się tylko zmieniał numer
     * na końcu:
     */
    strcpy(out_name, OUT_DIR);

    spi_fd = open(DEV_SPI, O_RDONLY);
    if (spi_fd == -1) {
        log_info(XERROR, "Nie można otworzyć '%s' do odczytu", DEV_SPI);
        return 1;
    }

    log_info(INFO, "Otwarto plik SPI: %s", DEV_SPI);

    while(1) {
        size_t i;

        log_info(INFO, "Oczekiwanie na %s ...", DEV_SPI);

        int x = read_byte(spi_fd, -1);
        if (x == -3) {
            /* EOF, koniec pliku, więc nie ma sensu kontynuować */
            log_info(ERROR, "Napotkano koniec pliku '%s', koniec.", DEV_SPI);
            close(spi_fd);
            free(out_name);
            return 1;
        }
        if (x < 0) {
            log_info(XERROR, "read_byte: Błąd odczytu '%s'", DEV_SPI);
            continue;
        }
        /* przyszedł pierwszy bajt kolejnej ramki
         * zrobić nowy plik i zapisać zawartość */

        /* nazwa pliku:
         * out_name[out_dir_length] to pierwszy znak za ścieżką katalogu,
         * tzn. pierwszy znak numeru który chcemy wstawić/zmienić
         */
        snprintf(&out_name[out_dir_length], digit_count, FORMAT_MSG_NUMBER, msg_number);

        /* nazwa ok, otwieramy plik do zapisu */

        out_fd = open(out_name, O_WRONLY|O_CREAT|O_TRUNC, OUT_FILE_MODE);
        if (out_fd == -1) {
            /* błąd w open() */
            log_info(XERROR, "open(): Nie można otworzyć '%s' do zapisu", out_name);
            continue;
        }
        if(write(out_fd, &x, 1) <= 0) {
            log_info(XERROR, "write(): Błąd zapisu '%s'", out_name);
        }

        /* 1 bajt zapisano, teraz pora na pozostałe */
        for(i = 1; i < MESSAGE_SIZE; i++) {
            x = read_byte(spi_fd, LORA_TIMEOUT);
            if (x == -1) {
                /* timeout */
                log_info(XERROR, "read_byte(): TIMEOUT - "
                        "Nie otrzymano kolejnego bajtu ramki");
                log_info(INFO, "LORA_TIMEOUT = %d", LORA_TIMEOUT);
                log_info(INFO, "Nr ramki: " FORMAT_MSG_NUMBER, msg_number);
                log_info(INFO, "Otrzymano %zu z %zu B", i, (size_t)MESSAGE_SIZE);
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
                close(out_fd);
                close(spi_fd);
                free(out_name);
                return 1;
            }

            if(write(out_fd, &x, 1) <= 0) {
                /* błąd zapisu */
                log_info(XERROR, "write(): Błąd zapisu '%s'", out_name);
                break;
            }
        } /* end for */
        /* zapisano całą ramkę, można zamknąć plik */
        close(out_fd);
        out_fd = -1;
        if (i == MESSAGE_SIZE) {
            /* nie przerwano for(), a więc nie było błędu I/O,
             * można wyświetlić info że zapisano i jest ok
             */
            log_info(INFO, "Zapisano ramkę LoRa do pliku '%s'", out_name);
        }
        msg_number++;
    }
    close(spi_fd);
    free(out_name);
    return 0;
}

