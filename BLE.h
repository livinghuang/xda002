#ifndef BLE_H
#define BLE_H
#include "global.h"
void ble_init();
void ble_process(void);
void send_to_ble(const char *data);
bool receive_from_ble(char *data);
void ble_stop();
void ble_name(const char *name);
// void bleSend(const char *data, uint8_t len);
#endif