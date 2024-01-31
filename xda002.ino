#include "global.h"

void setup()
{
  Serial.begin(115200);
  esp_sleep_wakeup_cause_t wakeup_reason = print_wakeup_reason();
  if (wakeup_reason == ESP_SLEEP_WAKEUP_UNDEFINED)
  {
    get_chip_id();
    ble_init();
    run_with_time_escape(30000, ble_process, ble_stop);
  }

  // lora_init();
  // rs485_init();
}

void loop()
{
  // lora_process();
  // rs485_process();
}
