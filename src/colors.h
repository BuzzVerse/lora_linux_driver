// how to use
// you need to print one of these colors before the text you want in color (for example C_RED),
// then you need to change it back to the default (C_DEFAULT)
// printf("%sAhoj%s Przygodo%s", C_RED, C_BLUE, C_DEFAULT);
// or
// printf(C_RED); printf("Ahoj "); printf(C_BLUE); printf("Przygodo"); printf(C_DEFAULT);

#define C_DEFAULT "\033[0m"
#define C_BLACK "\033[0;30m"
#define C_RED "\033[0;31m"
#define C_GREEN "\033[0;32m"
#define C_YELLOW "\033[0;33m"
#define C_BLUE "\033[0;34m"
#define C_PURPLE "\033[0;35m"
#define C_CYAN "\033[0;36m"
#define C_WHITE "\033[0;37m"
