#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include "log_info.h"

static unsigned int _loginfo_getprefixsize(int what) {
    unsigned int prefix_length = 0;
    /* bardziej profesjonalnie byłoby coś w stylu strlen(),
     * ale trzeba by wtedy uwzględnić UTF-8, bo wychodzą złe
     * wyniki dla [BŁĄD]
     * więc jest tak:
     */
    if (what & ERROR) prefix_length += 7;
    if (what & WARN) prefix_length += 8;
    if (what & INFO) prefix_length += 7;
    return prefix_length;
}

int log_info(int what, const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    if (what & ERROR) fprintf(stderr, "[BŁĄD] ");
    if (what & WARN) fprintf(stderr, "[UWAGA] ");
    if (what & INFO) fprintf(stderr, "[INFO] ");

    vfprintf(stderr, format, arg);
    fputc('\n', stderr);

    if (what & ERR_STATUS) {
        unsigned int prefix_length = _loginfo_getprefixsize(what);
        if (prefix_length) fprintf(stderr, "%*c", prefix_length, ' ');
        fprintf(stderr, "Status errno: %d (%s)\n",
                errno, strerror(errno));
    }

    return 0;
}


int _printwhere(int what, const char *file, unsigned long line) {
    unsigned int prefix_length = _loginfo_getprefixsize(what);
    if (prefix_length) fprintf(stderr, "%*c", prefix_length, ' ');

    fprintf(stderr, "Plik: %s  Linia: %lu\n", file, line);
    return 0;
}
