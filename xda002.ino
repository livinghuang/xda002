#include "global.h"

void bsp_init(void)
{
  pinMode(pVext, OUTPUT);
  digitalWrite(pVext, HIGH);
  Serial.println("BSP init");
}

uint32_t timestamp;
esp_sleep_wakeup_cause_t wakeup_reason;
void setup()
{

  // Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);
  Serial.begin(115200);
  Serial.println("Hello XDA002");
  Serial.flush();
  generate_lorawan_settings_by_chip_id();
  Serial.flush();
  generate_ble_name();
  Serial.flush();
  bsp_init();
  pinMode(pVext, OUTPUT);
  digitalWrite(pVext, LOW);
  wakeup_reason = print_wakeup_reason();

  while (!flash_init())
  {
    Serial.println("Flash init failed");
    FORMAT_FFAT = true;
  }
  if (wakeup_reason == ESP_SLEEP_WAKEUP_UNDEFINED)
  {
    Serial.println("Wakeup was not caused by deep sleep");
    Serial.flush();
    extern bool ble_processing_flag;
    ble_processing_flag = true;
    ble_init(10000);
    while (ble_processing_flag)
    {
      ble_process();
    }
  }
  timestamp = millis();
  lorawan_init();
}

void loop()
{
  lorawan_process();
}