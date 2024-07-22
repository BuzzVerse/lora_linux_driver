#ifndef MQTT_CONFIG_H
#define MQTT_CONFIG_H

#include <unistd.h>

#define MQTT_CONFIG_FILE "/etc/mqtt_config"

typedef struct {
    char ip[32];
    char login[64];
    char password[64];
    char topic[128];
} mqtt_config;

void trim_whitespace(char* str);

int parse_config(const char* filename, mqtt_config* config);

void mqtt_publish(char* msg, mqtt_config* config);

int parse_config(const char* filename, mqtt_config* config); 

#endif // MQTT_CONFIG_H

