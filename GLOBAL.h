#ifndef GLOBAL_H
#define GLOBAL_H

#include "Arduino.h"
#include "esp_wifi.h"
#include "esp_system.h"

#include "esp_system.h"
#include "LoRaWan_APP.h"
#include "WiFi.h"
#include "Wire.h"
#include "esp_sleep.h"
#include <esp_timer.h>
#include <mbedtls/sha256.h>
#include "FS.h"
#include "FFat.h"

#include "bat.h"
#include "ble.h"
#include "lorawan.h"
#include "at.h"
#include "rs485.h"
#include "flash.h"
#include "storage.h"
#include "bsp.h"

#define DATA_LEN 17

typedef struct sensor_data
{
  uint8_t battery_level;
  uint16_t humidity;
} sensor_data_t;
extern sensor_data_t sensor_data;
extern storage_info_struct storage_info;
extern enum Storage_Operation storage_operation;
extern bool deviceConnected;
extern byte responseBuffer[];
extern char board_information_string[];
extern char storage_information_string[];
extern char ble_buffer[];
void printHex(byte *data, int length);

extern bool reset_run_with_time_escape;
void run_with_time_escape(uint64_t escape_period_ms, void (*callback)(), void (*stop_callback)());
esp_sleep_wakeup_cause_t print_wakeup_reason();
bool is_a_string(const char *ptr, const int maxSize);
uint64_t get_chip_id();
void prepareData(void);
#endif