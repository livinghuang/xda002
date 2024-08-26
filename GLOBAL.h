#ifndef GLOBAL_H
#define GLOBAL_H

#include "Arduino.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include <time.h>
#include "esp_system.h"

#include "LoRaWan_APP.h"
// #include "WiFi.h"
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
// #include "storage.h"
#include "bsp.h"

#define DATA_LEN 17

// Define the struct
typedef struct WaterMeterData
{
  // 總累積: Total Accumulation
  byte totalAccumulationValue[8];

  // 電力不足天數: Power Insufficiency Days
  uint8_t powerInsufficiencyDays[2];

  // Device Name
  byte deviceName[6];
} WaterMeterData_t;

typedef struct sensor_data
{
  uint8_t main_battery_level;
  uint8_t reserve[3];
  WaterMeterData_t water_meter_data;
} sensor_data_t;
extern WaterMeterData meterData;
extern sensor_data_t sensor_data;
// extern storage_info_struct storage_info;
// extern enum Storage_Operation storage_operation;
extern bool deviceConnected;
extern byte responseBuffer[];
// extern char board_information_string[];
// extern char storage_information_string[];
extern char ble_buffer[];
extern uint32_t timestamp;
extern esp_sleep_wakeup_cause_t wakeup_reason;
void printHex(byte *data, int length);

extern bool reset_run_with_time_escape;
// void run_with_time_escape(uint64_t escape_period_ms, void (*callback)(), void (*stop_callback)());
esp_sleep_wakeup_cause_t print_wakeup_reason();
bool is_a_string(const char *ptr, const int maxSize);
uint64_t get_chip_id();
uint32_t simpleHash(const char *str);
void sleep_test(void);
void print_bytes_reverse(uint8_t *data, int length);
void print_bytes(uint8_t *data, int length);
#endif