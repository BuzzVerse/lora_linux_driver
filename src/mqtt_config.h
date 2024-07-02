#ifndef MQTT_CONFIG_H
#define MQTT_CONFIG_H

#include <unistd.h>

typedef struct {
    char ip[17];
    int port;
    char login[64];
    char password[64];
    char topic[64];
} mqtt_config;

void read_mqtt_config(mqtt_config* config);
#endif // MQTT_CONFIG_H
