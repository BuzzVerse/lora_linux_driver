#include <unistd.h>

typedef struct {
    char ip[17];
    char port[6];
    char login[64];
    char password[64];
} mqtt_config;
