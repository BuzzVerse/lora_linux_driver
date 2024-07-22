#include "../src/lora.h"
#include "packet/packet.h"
#include <assert.h>
#include "unity.h"
#include <stdlib.h>

void setUp(void) {

}

void tearDown(void) {

}

void test_lora_receive(void) {
    packet_t packet = {0};
    TEST_ASSERT_TRUE(lora_receive(&packet) == LORA_OK);
    TEST_ASSERT_TRUE(packet.version == 0x33);
    TEST_ASSERT_TRUE(packet.id == 0x22);
    TEST_ASSERT_TRUE(packet.msgID == 0x11);
    TEST_ASSERT_TRUE(packet.msgCount == 0x00);
    TEST_ASSERT_TRUE(packet.dataType == BMA400); 
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
