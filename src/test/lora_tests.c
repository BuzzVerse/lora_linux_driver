#include "unity/unity.h"
#include "lora.h"
#include "packet/packet.h"

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

}

void test_temp_init(void) {

}

void test_pack_packet(void) {

}

void test_get_data_size(void) {

}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_spidev_open);
    RUN_TEST(test_spidev_close);
    RUN_TEST(test_buffer_to_string);
    RUN_TEST(test_loginfo);
    RUN_TEST(test_lora_receive);
    RUN_TEST(test_temp_init);
    RUN_TEST(test_pack_packet);
    RUN_TEST(test_get_data_size);
    return UNITY_END();
}
