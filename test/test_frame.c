/* Program tworzący plik z testowymi, prawidłowymi ramkami LoRa */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../lora_frame.h"
#include "../sha.h"


void help(void){
    puts("Użycie: test_frame PLIK ILE_RAMEK SLEEP");
    puts("Parametry:");
    puts("PLIK        nazwa pliku, do którego mają być zapisane ramki");
    puts("            przed rozpoczęciem, poprzednia zawartość tego pliku");
    puts("            jest usuwana");
    puts("ILE_RAMEK   ile ramek ma zostać wygenerowanych");
    puts("            0 oznacza, że będą generowane aż do naciśnięcia Ctrl-C");
    puts("            (tzn. do otrzymania syngału SIGINT)");
    puts("SLEEP       OPCJONALNY: po zapisaniu każdej ramki czeka podaną");
    puts("            ilość milisekund");
    puts("");
    puts("np.:");
    puts("test_frame ramki.bin 64");
    puts("    - generuje 64 ramki LoRa i zapisuje je do pliku ramki.bin");
    puts("test_frame qwerty.dat 0");
    puts("    - dopóki ktoś nie naciśnie Ctrl-C, generuje ramki LoRa");
    puts("    i zapisuje je do pliku qwerty.dat");
    puts("test_frame test.bin 512 1000");
    puts("    - co 1 sekundę dopisuje kolejną ramkę do pliku test.bin");
    puts("    - kończy po zapisaniu 512 ramek");
    puts("test_frame /dev/pts/1 0 5000");
    puts("    - co 5 sekund dopisuje kolejną ramkę do terminala /dev/pts/1");
    puts("    - kończy po otrzymaniu Ctrl-C (SIGINT)");
}

void sleep_ms(long ms) {
    /* czeka podaną ilość milisekund */
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, &ts);
}


int main(int argc, const char **argv){
    
    FILE *f;
    long frame_count;
    long frame_number = 0;
    lora_frame frame;
    uint8_t data_size;
    unsigned int sleep_time = 0; /* czas w milisekundach */
    size_t header_size = sizeof(frame.MAC) + sizeof(frame.packet_type) + sizeof(frame.packet_number) + sizeof(frame.packet_count) + sizeof(frame.data_count);

    /* argv[1] = nazwa pliku, argv[2] = ile ramek,
     * opcjonalnie argv[3] to sleep
     */
    if (argc < 3) {
        help();
        return 1;
    }

    if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-help")) {
        help();
        return 0;
    }

    frame_count = strtol(argv[2], NULL, 10);

    if (argc > 3) sleep_time = strtol(argv[3], NULL, 10);

    f = fopen(argv[1], "wb");
    if (!f) return 1;

    srand(time(NULL));

    /* if frame_count to 0: generować w nieskończoność */
    while(frame_number < frame_count || !frame_count) {
        for(size_t i = 0; i < sizeof(frame); i++) ((uint8_t*)&frame)[i] = 0;

        data_size = 4;

        frame.MAC[0] = 'M';
        frame.MAC[1] = 'M';
        frame.MAC[2] = 'M';
        frame.MAC[3] = 'M';
        frame.MAC[4] = 'M';
        frame.MAC[5] = 'M';
        frame.packet_type = 0;
        frame.packet_number = 1;
        frame.packet_count = 1;
        frame.data_count = data_size;

        for(size_t i = 0; i < data_size; i++)
            frame.data[i] = 'D';

        sha_create(&frame, &frame.SHA256);

        printf("SHA: %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X\n",
            frame.SHA256[0],
            frame.SHA256[1],
            frame.SHA256[2],
            frame.SHA256[3],
            frame.SHA256[4],
            frame.SHA256[5],
            frame.SHA256[6],
            frame.SHA256[7],
            frame.SHA256[8],
            frame.SHA256[9],
            frame.SHA256[10],
            frame.SHA256[11],
            frame.SHA256[12],
            frame.SHA256[13],
            frame.SHA256[14],
            frame.SHA256[15],
            frame.SHA256[16],
            frame.SHA256[17],
            frame.SHA256[18],
            frame.SHA256[19],
            frame.SHA256[20],
            frame.SHA256[21],
            frame.SHA256[22],
            frame.SHA256[23],
            frame.SHA256[24],
            frame.SHA256[25],
            frame.SHA256[26],
            frame.SHA256[27],
            frame.SHA256[28],
            frame.SHA256[29],
            frame.SHA256[30],
            frame.SHA256[31] );


        if (!fwrite(&frame, header_size, 1, f)) goto exit_error;
        if (!fwrite(&frame.data, frame.data_count, 1, f)) goto exit_error;
        if (!fwrite(&frame.SHA256, sizeof(frame.SHA256), 1, f)) goto exit_error;

        /* żeby nie doszło do sytuacji że przez buforowanie coś się nie
         * zapisze od razu:
         */
        fflush(f);

        /* jeżeli podano sleep, to czekać: */
        if (sleep_time) sleep_ms(1000);

        frame_number++;
    }
    fclose(f);
    return 0;

exit_error:
    fprintf(stderr, "Błąd zapisu.\n");
    fclose(f);
    return 1;
}

