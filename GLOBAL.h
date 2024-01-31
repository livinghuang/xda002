#ifndef GLOBAL_H
#define GLOBAL_H

#define USE_BLE

#include "Arduino.h"
#include "esp_system.h"
#include "LoRaWan_APP.h"
#include "bsp.h"
#include "WiFi.h"
#include "Wire.h"
#include "esp_sleep.h"
#include <esp_timer.h>
#include <mbedtls/sha256.h>

#include "bat.h"
#include "ble.h"
#include "lorawan.h"
#include "AT.h"
#include "RS485.h"

extern bool deviceConnected;

void printHex(byte *data, int length);



extern bool reset_run_with_time_escape;
void run_with_time_escape(uint64_t escape_period_ms, void (*callback)(), void (*stop_callback)());
esp_sleep_wakeup_cause_t print_wakeup_reason();
uint64_t get_chip_id();
#endif