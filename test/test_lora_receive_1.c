#include "../src/lora.h"
#include "packet/packet.h"
#include <assert.h>
#include "unity.h"
#include <stdlib.h>

void setUp(void) {

}

void tearDown(void) {

}

// void test_buffer_to_string(void) {
//     {
//         uint8_t buffer[7] = {1, 2, 3, 4, 5, 6 ,7};
//         size_t buff_len = 7;
//         char destination[64];
//         size_t dest_len = 64;
//         buffer_to_string(buffer, buff_len, destination, dest_len);

//         // TEST_ASSERT_EQUAL_STRING(
//         //         "version: 0x01, id: 0x02, msgID: 0x03, msgCount: 0x04, dataType: 0x05, data: 0x06 0x07 ",
//         //         destination
//         //         );
//     }
//     {
//         uint8_t buffer[7] = {1, 2, 3, 4, 5, 6 ,7};
//         size_t buff_len = 7;
//         char destination[64];
//         size_t dest_len = 64;
//         buffer_to_string(buffer, NULL, destination, NULL);

//         // TEST_ASSERT_EQUAL_STRING(
//         //         "",
//         //         destination
//         //         );
//     }
// }

// void test_loginfo(void) {
//     // TEST_ASSERT_TRUE(loginfo("[TEST] Log from testing"));
// }

void test_lora_receive(void) {
    packet_t packet = {0};
    TEST_ASSERT_TRUE(lora_receive(&packet) == LORA_OK);
    TEST_ASSERT_TRUE(packet.version == 0x33);
    TEST_ASSERT_TRUE(packet.id == 0x22);
    TEST_ASSERT_TRUE(packet.msgID == 0x11);
    TEST_ASSERT_TRUE(packet.msgCount == 0x00);
    TEST_ASSERT_TRUE(packet.dataType == BME280); 
    TEST_ASSERT_TRUE(packet.data[0] == 23);
    TEST_ASSERT_TRUE(packet.data[1] == 4);
    TEST_ASSERT_TRUE(packet.data[2] == 56);
}

int main(void) {
    UNITY_BEGIN();  
    RUN_TEST(test_lora_receive);
    return UNITY_END();
    // test_lora_receive();
}
