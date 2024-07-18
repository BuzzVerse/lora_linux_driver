/* AUTOGENERATED FILE. DO NOT EDIT. */
#ifndef _MOCKLORA_DRIVER_H
#define _MOCKLORA_DRIVER_H

#include "unity.h"
#include "lora_driver.h"

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

void Mocklora_driver_Init(void);
void Mocklora_driver_Destroy(void);
void Mocklora_driver_Verify(void);




#define lora_write_reg_ExpectAndReturn(reg, val, cmock_retval) lora_write_reg_CMockExpectAndReturn(__LINE__, reg, val, cmock_retval)
void lora_write_reg_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t reg, uint8_t val, lora_status_t cmock_to_return);
#define lora_write_reg_buffer_ExpectAndReturn(reg, val, len, cmock_retval) lora_write_reg_buffer_CMockExpectAndReturn(__LINE__, reg, val, len, cmock_retval)
void lora_write_reg_buffer_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t reg, uint8_t* val, uint8_t len, lora_status_t cmock_to_return);
#define lora_read_reg_ExpectAndReturn(reg, val, cmock_retval) lora_read_reg_CMockExpectAndReturn(__LINE__, reg, val, cmock_retval)
void lora_read_reg_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t reg, uint8_t* val, lora_status_t cmock_to_return);
#define lora_read_reg_buffer_ExpectAndReturn(reg, val, len, cmock_retval) lora_read_reg_buffer_CMockExpectAndReturn(__LINE__, reg, val, len, cmock_retval)
void lora_read_reg_buffer_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t reg, uint8_t* val, uint8_t len, lora_status_t cmock_to_return);
#define lora_explicit_header_mode_ExpectAndReturn(cmock_retval) lora_explicit_header_mode_CMockExpectAndReturn(__LINE__, cmock_retval)
void lora_explicit_header_mode_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, lora_status_t cmock_to_return);
#define lora_implicit_header_mode_ExpectAndReturn(size, cmock_retval) lora_implicit_header_mode_CMockExpectAndReturn(__LINE__, size, cmock_retval)
void lora_implicit_header_mode_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t size, lora_status_t cmock_to_return);
#define lora_idle_mode_ExpectAndReturn(cmock_retval) lora_idle_mode_CMockExpectAndReturn(__LINE__, cmock_retval)
void lora_idle_mode_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, lora_status_t cmock_to_return);
#define lora_sleep_mode_ExpectAndReturn(cmock_retval) lora_sleep_mode_CMockExpectAndReturn(__LINE__, cmock_retval)
void lora_sleep_mode_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, lora_status_t cmock_to_return);
#define lora_receive_mode_ExpectAndReturn(cmock_retval) lora_receive_mode_CMockExpectAndReturn(__LINE__, cmock_retval)
void lora_receive_mode_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, lora_status_t cmock_to_return);
#define lora_set_tx_power_ExpectAndReturn(level, cmock_retval) lora_set_tx_power_CMockExpectAndReturn(__LINE__, level, cmock_retval)
void lora_set_tx_power_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t level, lora_status_t cmock_to_return);
#define lora_set_frequency_ExpectAndReturn(frequency, cmock_retval) lora_set_frequency_CMockExpectAndReturn(__LINE__, frequency, cmock_retval)
void lora_set_frequency_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, long frequency, lora_status_t cmock_to_return);
#define lora_set_spreading_factor_ExpectAndReturn(sf, cmock_retval) lora_set_spreading_factor_CMockExpectAndReturn(__LINE__, sf, cmock_retval)
void lora_set_spreading_factor_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t sf, lora_status_t cmock_to_return);
#define lora_get_spreading_factor_ExpectAndReturn(sf, cmock_retval) lora_get_spreading_factor_CMockExpectAndReturn(__LINE__, sf, cmock_retval)
void lora_get_spreading_factor_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t* sf, lora_status_t cmock_to_return);
#define lora_set_dio_mapping_ExpectAndReturn(dio, mode, cmock_retval) lora_set_dio_mapping_CMockExpectAndReturn(__LINE__, dio, mode, cmock_retval)
void lora_set_dio_mapping_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t dio, uint8_t mode, lora_status_t cmock_to_return);
#define lora_get_dio_mapping_ExpectAndReturn(dio, mapping, cmock_retval) lora_get_dio_mapping_CMockExpectAndReturn(__LINE__, dio, mapping, cmock_retval)
void lora_get_dio_mapping_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t dio, uint8_t* mapping, lora_status_t cmock_to_return);
#define lora_set_bandwidth_ExpectAndReturn(sbw, cmock_retval) lora_set_bandwidth_CMockExpectAndReturn(__LINE__, sbw, cmock_retval)
void lora_set_bandwidth_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t sbw, lora_status_t cmock_to_return);
#define lora_get_bandwidth_ExpectAndReturn(sbw, cmock_retval) lora_get_bandwidth_CMockExpectAndReturn(__LINE__, sbw, cmock_retval)
void lora_get_bandwidth_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t* sbw, lora_status_t cmock_to_return);
#define lora_set_coding_rate_ExpectAndReturn(denominator, cmock_retval) lora_set_coding_rate_CMockExpectAndReturn(__LINE__, denominator, cmock_retval)
void lora_set_coding_rate_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t denominator, lora_status_t cmock_to_return);
#define lora_get_coding_rate_ExpectAndReturn(cr, cmock_retval) lora_get_coding_rate_CMockExpectAndReturn(__LINE__, cr, cmock_retval)
void lora_get_coding_rate_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t* cr, lora_status_t cmock_to_return);
#define lora_set_preamble_length_ExpectAndReturn(length, cmock_retval) lora_set_preamble_length_CMockExpectAndReturn(__LINE__, length, cmock_retval)
void lora_set_preamble_length_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, long length, lora_status_t cmock_to_return);
#define lora_get_preamble_length_ExpectAndReturn(preamble, cmock_retval) lora_get_preamble_length_CMockExpectAndReturn(__LINE__, preamble, cmock_retval)
void lora_get_preamble_length_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, long* preamble, lora_status_t cmock_to_return);
#define lora_set_sync_word_ExpectAndReturn(sw, cmock_retval) lora_set_sync_word_CMockExpectAndReturn(__LINE__, sw, cmock_retval)
void lora_set_sync_word_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t sw, lora_status_t cmock_to_return);
#define lora_enable_crc_ExpectAndReturn(cmock_retval) lora_enable_crc_CMockExpectAndReturn(__LINE__, cmock_retval)
void lora_enable_crc_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, lora_status_t cmock_to_return);
#define lora_disable_crc_ExpectAndReturn(cmock_retval) lora_disable_crc_CMockExpectAndReturn(__LINE__, cmock_retval)
void lora_disable_crc_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, lora_status_t cmock_to_return);
#define lora_driver_init_ExpectAndReturn(cmock_retval) lora_driver_init_CMockExpectAndReturn(__LINE__, cmock_retval)
void lora_driver_init_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, lora_status_t cmock_to_return);
#define lora_send_packet_ExpectAndReturn(buf, size, cmock_retval) lora_send_packet_CMockExpectAndReturn(__LINE__, buf, size, cmock_retval)
void lora_send_packet_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t* buf, uint8_t size, lora_status_t cmock_to_return);
#define lora_receive_packet_ExpectAndReturn(buf, return_len, size, cmock_retval) lora_receive_packet_CMockExpectAndReturn(__LINE__, buf, return_len, size, cmock_retval)
void lora_receive_packet_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t* buf, uint8_t* return_len, uint8_t size, lora_status_t cmock_to_return);
#define lora_received_ExpectAndReturn(received, crc_error, cmock_retval) lora_received_CMockExpectAndReturn(__LINE__, received, crc_error, cmock_retval)
void lora_received_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, bool* received, bool* crc_error, lora_status_t cmock_to_return);
#define lora_get_irq_ExpectAndReturn(irq_flags, cmock_retval) lora_get_irq_CMockExpectAndReturn(__LINE__, irq_flags, cmock_retval)
void lora_get_irq_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t* irq_flags, lora_status_t cmock_to_return);
#define lora_packet_lost_ExpectAndReturn(cmock_retval) lora_packet_lost_CMockExpectAndReturn(__LINE__, cmock_retval)
void lora_packet_lost_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t cmock_to_return);
#define lora_packet_rssi_ExpectAndReturn(rssi, cmock_retval) lora_packet_rssi_CMockExpectAndReturn(__LINE__, rssi, cmock_retval)
void lora_packet_rssi_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t* rssi, lora_status_t cmock_to_return);
#define lora_packet_snr_ExpectAndReturn(snr, cmock_retval) lora_packet_snr_CMockExpectAndReturn(__LINE__, snr, cmock_retval)
void lora_packet_snr_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, uint8_t* snr, lora_status_t cmock_to_return);
#define lora_close_Expect() lora_close_CMockExpect(__LINE__)
void lora_close_CMockExpect(UNITY_LINE_TYPE cmock_line);
#define lora_dump_registers_ExpectAndReturn(cmock_retval) lora_dump_registers_CMockExpectAndReturn(__LINE__, cmock_retval)
void lora_dump_registers_CMockExpectAndReturn(UNITY_LINE_TYPE cmock_line, lora_status_t cmock_to_return);

#ifdef __cplusplus
}
#endif

#if defined(__GNUC__) && !defined(__ICC) && !defined(__TMS470__)
#if __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 6 || (__GNUC_MINOR__ == 6 && __GNUC_PATCHLEVEL__ > 0)))
#pragma GCC diagnostic pop
#endif
#endif

#endif