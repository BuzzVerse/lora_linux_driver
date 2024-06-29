#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <string.h>

// #include "mqtt/mqtt.h"
// #include "mqtt/posix_sockets.h"
#include "driver/lora_driver.h"
#include "packet/packet.h"

// functions from bbb_api_impl.c
extern void spidev_close();
extern int spidev_open(char* dev);
extern void print_buffer(uint8_t* buf, uint8_t len);

// void publish_callback(void** unused, struct mqtt_response_publish *published) {}
// void* client_refresher(void* client) {}
// void exit_mqtt(int status, int sockfd, pthread_t *client_daemon) {
//     if (sockfd != -1) close(sockfd);
//     if (client_daemon != NULL) pthread_cancel(*client_daemon);
//     exit(status);
// }

void handle_sigint(int sig) {
    printf("Caught signal %d (SIGINT), cleaning up...\n", sig);

    lora_sleep_mode();

    spidev_close();

    exit(0);
}

int main(int argc, char* argv[])
{
    if(argc != 2) {
        printf("Invalid number of arguments\nUsage: sudo ./lora_rx [SPI device number: 0 | 1]\n");
        return -1;
    }

    char* device;

    if(strcmp(argv[1], "0") == 0) {
        device = "/dev/spidev0.0";
    } else if(strcmp(argv[1], "1") == 0) {
        device = "/dev/spidev1.0";
    } else {
        printf("Invalid SPI device\n");
        return -2;
    }

    // signal handler for CTRL-C
    signal(SIGINT, handle_sigint);

    if(spidev_open(device) == -1) {
        return -1; // exit if fd fails to open
    }

    lora_driver_init();

    lora_idle_mode();

    lora_set_frequency(433 * 1e6);
    lora_set_bandwidth(4);
    lora_set_coding_rate(8);
    lora_set_spreading_factor(12);
    lora_enable_crc();

    lora_receive_mode();

    packet_t packet;
    bool received = false;
    bool crc_error = false;
    uint8_t irq;

    // // mqtt stuff
    // char* ip = "158.180.60.2";
    // char* port = "1883";
    // int sockfd = open_nb_socket(ip, port);
    // if (sockfd == -1) {
    //     perror("Failed to open socket: ");
    //     exit_mqtt(EXIT_FAILURE, sockfd, NULL);
    // }

    // struct mqtt_client client;
    // uint8_t mqtt_out_buff[256];
    // uint8_t mqtt_in_buff[64];
    // mqtt_init(&client, sockfd, mqtt_out_buff, sizeof(mqtt_out_buff), mqtt_in_buff, sizeof(mqtt_in_buff), publish_callback);
    // const char* client_id = NULL;
    // uint8_t connect_flags = MQTT_CONNECT_CLEAN_SESSION;
    // char* username = "admin";
    // char* password = "";
    // mqtt_connect(&client, "MQTT", NULL, NULL, 0, username, password, 0, 400);

    // if (client.error != MQTT_OK) {
    //     fprintf(stderr, "error: %s\n", mqtt_error_str(client.error));
    //     exit_mqtt(EXIT_FAILURE, sockfd, NULL);
    // }

    // pthread_t client_daemon;
    // if(pthread_create(&client_daemon, NULL, client_refresher, &client)) {
    //     fprintf(stderr, "Failed to start client daemon.\n");
    //     exit_mqtt(EXIT_FAILURE, sockfd, NULL);

    // }

    while(1) {
        lora_received(&received, &crc_error);

        if(received) {
            lora_get_irq(&irq);
            char msg[100];

            if(crc_error) {
                printf("CRC error\n");
                crc_error = false;
            } else {
                uint8_t return_len;
                lora_receive_packet((uint8_t*)&packet, &return_len, PACKET_SIZE); // puts LoRa in idle mode!!!
                // // unpack data
                // float received_temp = ((float)((int8_t)packet.data[0]) / 2.0);
                // float received_press = (float)(1000 + (int8_t)packet.data[1]);
                // float received_hum = (float)packet.data[2];
                // // write message
                // sprintf(msg, "{\"temperature\":%.2f, \"pressure\":%.2f, \"humidity\":%.2f}",
                //         received_temp, received_press, received_hum);
                // mqtt_publish(&client, "tele/lora/SENSOR_SPANISH", msg, strlen(msg) + 1, MQTT_PUBLISH_QOS_0);

                // if (client.error != MQTT_OK) {
                //     fprintf(stderr, "error: %s\n", mqtt_error_str(client.error));
                //     exit_mqtt(EXIT_FAILURE, sockfd, &client_daemon);
                // }
                print_buffer((uint8_t*)&packet, return_len);

                lora_receive_mode();
            }
            
            received = false;
        }
    }
    // exit_mqtt(EXIT_FAILURE, sockfd, &client_daemon);

    lora_sleep_mode();

    spidev_close();
}
