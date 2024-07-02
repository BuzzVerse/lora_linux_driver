#include "mqtt_config.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void read_mqtt_config(mqtt_config* config) {
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("/etc/mqtt_config", "r");
    if (fp == NULL) {
        printf("Cannot read mqtt config file at /etc/mqtt_config");
        exit(-1);
    }

    // copy config to struct
    while ((read = getline(&line, &len, fp)) != -1) {
        if (strncmp(line, "ip", 2)) {
            strcpy(config->ip, strstr(line, "=") + 1);
        } else if (strncmp(line, "port", 4)) {
            char* end;
            config->port = (int) strtol(strstr(line, "=") + 1, &end, 10);
        } else if (strncmp(line, "login", 5)) {
            strcpy(config->login, strstr(line, "=") + 1);
        } else if (strncmp(line, "password", 8)) {
            strcpy(config->password, strstr(line, "=") + 1);
        } else if (strncmp(line, "topic", 5)) {
            strcpy(config->topic, strstr(line, "=") + 1);
        }
    }

    fclose(fp);
    if (line) { free(line); }
}

