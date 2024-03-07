#ifndef LOG_INFO_H
#define LOG_INFO_H

/*
 * Funkcje do zapisywania loga z działania programu
 */

#include <stdarg.h>
#include <stdio.h>

#define ERROR       0x01
#define WARN        0x02
#define INFO        0x04
#define ERR_STATUS  0x08
#define XERROR      ERROR | ERR_STATUS

/* log_info
 * wypisuje info o błędach albo komunikaty statusu działania programu
 * dokleja przejście do nowej linii na końcu komunikatu
 *
 * format komunikatu jak printf:
 * parametr format to string ze znacznikami w stylu %d, %s itd.
 * i dalej zmienne które mają zostać wklejone do komunikatu
 *
 *
 * parametr what może być jedną poniższych wartości:
 *
 * ERROR    komunikat jest errorem
 * WARN     komunikat jest warnem
 * INFO     komunikat jest informacyjny
 * ERR_STATUS   za podanym komunikatem w nowej linii wyświetla
 *          status errno i przypisaną do niego informację o błędzie
 * XERROR   = to samo co ERROR | ERR_STATUS
 *
 * wartość 0 oznacza, że podany komunikat ma być dopisany do loga bez
 * dodatków w stylu "[INFO]", "[BŁĄD]", itd.
 */
int log_info(int what, const char *format, ...);


int _printwhere(int what, const char *file, unsigned long line); /* private */

#define log_debug(...) \
    (log_info(INFO, __VA_ARGS__), _printwhere(INFO, __FILE__, __LINE__))

#endif
