#include "GLOBAL.h"
#include "Arduino.h"
#include "esp_system.h"

void setup()
{
  Serial.begin(115200);
  ble_init();
  lora_init();
}

void loop()
{
  // lora_process();
  ble_process();
}