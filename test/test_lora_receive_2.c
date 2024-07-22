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
    TEST_ASSERT_TRUE(lora_receive(&packet) != LORA_OK);
}

int main(void) {
    UNITY_BEGIN();  
    RUN_TEST(test_lora_receive);
    return UNITY_END();
    // test_lora_receive();
}
