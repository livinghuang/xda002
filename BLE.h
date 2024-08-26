#ifndef BLE_H
#define BLE_H
#include "global.h"

extern char ble_buffer[];
void ble_process(void);
void send_to_ble(const char *data);
void ble_stop();
void ble_name(const char *name);
void ble_init(unsigned long timeout);
void ble_send_data_ready(void);
void generate_ble_name(void);
// void bleSend(const char *data, uint8_t len);
#endif