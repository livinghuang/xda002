#include "global.h"

// initial loop is for power on stage, it could keep run a period time to wait user enter setting mode.
void initial_loop(void)
{
  ble_process();
  storage_process();
}
// initial loop stop is to stop initial loop.
void initial_loop_stop(void)
{
  ble_stop();
}

void setup()
{
  Serial.begin(115200);
  // generate_lorawan_parameters_by_chip_id();
  esp_sleep_wakeup_cause_t wakeup_reason = print_wakeup_reason();
  if (wakeup_reason == ESP_SLEEP_WAKEUP_UNDEFINED)
  {
    get_chip_id();
    board_init();
    ble_init();
    run_with_time_escape(30000, initial_loop, initial_loop_stop);
  }

  // rs485_init();
  // rs485_process();
  lorawan_init();

  delay(100);
}

void loop()
{
  // lorawan_process();

  delay(1000);
  // storage_process();
}
