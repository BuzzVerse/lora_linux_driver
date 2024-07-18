#include "mqtt_config.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void trim_whitespace(char *str) {
    char *end;

    // Trim leading space
    while(isspace((unsigned char)*str)) str++;

    // Trim trailing space
    if(*str == 0)  // All spaces
        return;

    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator
    *(end+1) = 0;
}

int parse_config(const char *filename, mqtt_config *config) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open config file");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *equals = strchr(line, '=');
        if (equals == NULL) {
            continue; // Ignore lines without '='
        }

        *equals = '\0';
        char *key = line;
        char *value = equals + 1;

        trim_whitespace(key);
        trim_whitespace(value);

        if (strcmp(key, "ip") == 0) {
            strncpy(config->ip, value, sizeof(config->ip) - 1);
            config->ip[sizeof(config->ip) - 1] = '\0';
        } else if (strcmp(key, "login") == 0) {
            strncpy(config->login, value, sizeof(config->login) - 1);
            config->login[sizeof(config->login) - 1] = '\0';
        } else if (strcmp(key, "password") == 0) {
            strncpy(config->password, value, sizeof(config->password) - 1);
            config->password[sizeof(config->password) - 1] = '\0';
        } else if (strcmp(key, "topic") == 0) {
            strncpy(config->topic, value, sizeof(config->topic) - 1);
            config->topic[sizeof(config->topic) - 1] = '\0';
        }
    }

    fclose(file);
    return 0;
}

void mqtt_publish(char* msg, mqtt_config* config) {
    char buffer[512];
    snprintf(buffer,512,
            "mosquitto_pub -h %s -P \'%s\' -u %s -t \'%s\' -m \'%s\'",
            config->ip, config->password, config->login, config->topic, msg);
    system(buffer);

}
