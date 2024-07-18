/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCKLORA_H
#define _MOCKLORA_H

#include "unity.h"
#include "lora.h"

/* Ignore the following warnings, since we are copying code */
#if defined(__GNUC__) && !defined(__ICC) && !defined(__TMS470__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 6 || (__GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ > 0)))
#pragma GCC diagnostic push
#endif
#if !defined(__clang__)
#pragma GCC diagnostic ignored "-Wpragmas"
#endif
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wduplicate-decl-specifier"
#endif

#ifdef __cplusplus
extern "C" {
#endif

void Mocklora_Init(void);
void Mocklora_Destroy(void);
void Mocklora_Verify(void);




#define spidev_open_ExpectAndReturn(dev, cmock_retval) spidev_open_CMockExpectAndReturn(__LINE__, dev, cmock_retval)
void spidev_open_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, const char* dev, int cmock_to_return);
#define spidev_close_Expect() spidev_close_CMockExpect(__LINE__)
void spidev_close_CMockExpect(UNITY_LINE_TYPE cmock_line);
#define print_buffer_Expect(buf, len) print_buffer_CMockExpect(__LINE__, buf, len)
void print_buffer_CMockExpect(UNITY_LINE_TYPE cmock_line, uint8_t* buf, uint8_t len);
#define buffer_to_string_Expect(buffer, buffer_size, destination, destination_size) buffer_to_string_CMockExpect(__LINE__, buffer, buffer_size, destination, destination_size)
void buffer_to_string_CMockExpect(UNITY_LINE_TYPE cmock_line, uint8_t* buffer, size_t buffer_size, char* destination, size_t destination_size);
#define loginfo_ExpectAndReturn(msg, cmock_retval) loginfo_CMockExpectAndReturn(__LINE__, msg, cmock_retval)
void loginfo_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, const char* msg, int cmock_to_return);
#define lora_receive_ExpectAndReturn(packet, cmock_retval) lora_receive_CMockExpectAndReturn(__LINE__, packet, cmock_retval)
void lora_receive_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, packet_t* packet, lora_status_t cmock_to_return);
#define temp_init_ExpectAndReturn(cmock_retval) temp_init_CMockExpectAndReturn(__LINE__, cmock_retval)
void temp_init_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, lora_status_t cmock_to_return);
#define pack_packet_Expect(buffer, packet, data_size) pack_packet_CMockExpect(__LINE__, buffer, packet, data_size)
void pack_packet_CMockExpect(UNITY_LINE_TYPE cmock_line, uint8_t* buffer, packet_t* packet, size_t data_size);
#define get_data_size_ExpectAndReturn(type, cmock_retval) get_data_size_CMockExpectAndReturn(__LINE__, type, cmock_retval)
void get_data_size_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, DataType type, size_t cmock_to_return);

#ifdef __cplusplus
}
#endif

#if defined(__GNUC__) && !defined(__ICC) && !defined(__TMS470__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 6 || (__GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ > 0)))
#pragma GCC diagnostic pop
#endif
#endif

#endif