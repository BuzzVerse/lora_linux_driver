#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include "log_info.h"

void log_info(int what, const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    if (what & ERROR) fprintf(stderr, "[BŁĄD] ");
    if (what & WARN) fprintf(stderr, "[UWAGA] ");
    if (what & INFO) fprintf(stderr, "[INFO] ");

    vfprintf(stderr, format, arg);
    fputc('\n', stderr);

    if (what & ERR_STATUS) {
        fprintf(stderr, "[INFO] Status errno: %d (%s)\n",
                errno, strerror(errno));
    }
}

