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
  generate_lorawan_parameters_by_chip_id();
  esp_sleep_wakeup_cause_t wakeup_reason = print_wakeup_reason();
  if (wakeup_reason == ESP_SLEEP_WAKEUP_UNDEFINED)
  {
    get_chip_id();
    board_init();
    ble_init();
    run_with_time_escape(30000, initial_loop, initial_loop_stop);
  }
  rs485_init();
  rs485_process();

  byte batteryLevel = getBatteryLevel();
  uint8_t heltec_rs485_soil_sensor_data_length = responseBuffer[2];
  uint8_t *heltec_rs485_soil_sensor_data_head = &responseBuffer[3];

  decode_soil(heltec_rs485_soil_sensor_data_head, heltec_rs485_soil_sensor_data_length);
  Serial.flush();
  appDataSize = 1 + heltec_rs485_soil_sensor_data_length;
  appData[0] = batteryLevel;
  memcpy(appData + 1, heltec_rs485_soil_sensor_data_head, heltec_rs485_soil_sensor_data_length);
  save_to_storage(&appData[0]);
  lorawan_init();
}

void loop()
{
  lorawan_process();
  storage_process();
}
