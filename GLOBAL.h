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
#include "FS.h"
#include "FFat.h"

#include "bat.h"
#include "ble.h"
#include "lorawan.h"
#include "at.h"
#include "RS485.h"
#include "soil.h"
#include "flash.h"
#include "storage.h"
#include "bsp.h"

enum Storage_Operation
{
  _INIT, // mount FFAT and show the total space and free space
  _INFO, // show the total space and free space and also show the storage files information
  _WRITE,// write data
  _READ,
  _CLR,
  _FORMAT,
  _NONE
};

extern enum Storage_Operation storage_operation;
extern bool deviceConnected;
extern byte responseBuffer[256];

void printHex(byte *data, int length);

extern bool reset_run_with_time_escape;
void run_with_time_escape(uint64_t escape_period_ms, void (*callback)(), void (*stop_callback)());
esp_sleep_wakeup_cause_t print_wakeup_reason();
uint64_t get_chip_id();
#endif