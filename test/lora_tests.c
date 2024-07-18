#include "mocks/unity.h"
#include "../src/lora.h"
#include "mocks/Mocklora.h"
#include "mocks/Mocklora_driver.h"
#include "packet/packet.h"
#include <stdlib.h>

void setUp(void) {

}

void tearDown(void) {

}

void test_spidev_open(void) {

}

void test_spidev_close(void) {

}

void test_buffer_to_string(void) {
    {
        uint8_t buffer[7] = {1, 2, 3, 4, 5, 6 ,7};
        size_t buff_len = 7;
        char destination[64];
        size_t dest_len = 64;
        buffer_to_string(buffer, buff_len, destination, dest_len);

        TEST_ASSERT_EQUAL_STRING(
                "version: 0x01, id: 0x02, msgID: 0x03, msgCount: 0x04, dataType: 0x05, data: 0x06 0x07 ",
                destination
                );
    }
    {
        uint8_t buffer[7] = {1, 2, 3, 4, 5, 6 ,7};
        size_t buff_len = 7;
        char destination[64];
        size_t dest_len = 64;
        buffer_to_string(buffer, NULL, destination, NULL);

        TEST_ASSERT_EQUAL_STRING(
                "",
                destination
                );
    }
}

void test_loginfo(void) {
    TEST_ASSERT_TRUE(loginfo("[TEST] Log from testing"));
}

void test_lora_receive(void) {
    uint8_t return_len = 0;
    uint8_t rssi = 0;
    uint8_t snr = 0;

    lora_receive_mode_ExpectAndReturn(0);


    uint8_t buffer[PACKET_SIZE]; 
    lora_receive_packet_ExpectAndReturn(buffer, &return_len, sizeof(buffer), 1);

    lora_packet_rssi_ExpectAndReturn(&rssi, 1);
    lora_packet_snr_ExpectAndReturn(&snr, 1);
    
    packet_t* packet = NULL;
    packet = (packet_t*)malloc(sizeof(packet_t));
    packet->version = 0x33;
    packet->id = 0x22;
    packet->msgID = 0x11;
    packet->msgCount = 0x00;
    packet->dataType = BME280; 
    packet->data[0] = 23;
    packet->data[1] = 4;
    packet->data[2] = 56;

    {
        bool received = true;
        bool crc_error = true;
        lora_received_ExpectAndReturn(&received, &crc_error, LORA_OK);
        TEST_ASSERT_TRUE(LORA_CRC_ERROR == lora_receive(packet));
    }
    {
        bool received = true;
        bool crc_error = false;
        lora_received_ExpectAndReturn(&received, &crc_error, LORA_OK);
        TEST_ASSERT_TRUE(LORA_OK == lora_receive(packet));
    }
    free(packet);
}

// void test_temp_init(void) {

// }

// void test_pack_packet(void) {

// }

// void test_get_data_size(void) {

// }

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_spidev_open);
    RUN_TEST(test_spidev_close);
    RUN_TEST(test_buffer_to_string);
    RUN_TEST(test_loginfo);
    RUN_TEST(test_lora_receive);
    // RUN_TEST(test_temp_init);
    // RUN_TEST(test_pack_packet);
    // RUN_TEST(test_get_data_size);
    return UNITY_END();
}
