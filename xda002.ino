#include "global.h"

// // initial loop is for power on stage, it could keep run a period time to wait user enter setting mode.
// void initial_loop(void)
// {
//   ble_process();
//   storage_process();
// }
// // initial loop stop is to stop initial loop.
// void initial_loop_stop(void)
// {
//   ble_stop();
// }

void bsp_init(void)
{
  // pinMode(DI, mode);
  // digitalWrite(DI, level);
  // pinMode(RO, mode);
  // digitalWrite(RO, level);
  // pinMode(RE_DE, mode);
  // digitalWrite(RE_DE, level);
  pinMode(pVext, OUTPUT);
  digitalWrite(pVext, HIGH);

  Serial.println("BSP init");
}

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("Hello XDA002");
  Serial.flush();
  bsp_init();
  esp_sleep_wakeup_cause_t wakeup_reason = print_wakeup_reason();
  delay(1000);
  if (wakeup_reason == ESP_SLEEP_WAKEUP_UNDEFINED)
  {
    Serial.println("Wakeup was not caused by deep sleep");
    Serial.flush();
    get_chip_id();
    flash_init();
    board_init();
    // ble_init();
    //   // run_with_time_escape(3000, initial_loop, initial_loop_stop);
  }
  // // uint8_t heltec_rs485_soil_sensor_data_length = responseBuffer[2];
  // // uint8_t *heltec_rs485_soil_sensor_data_head = &responseBuffer[3];

  // // decode_soil(heltec_rs485_soil_sensor_data_head, heltec_rs485_soil_sensor_data_length);
  // Serial.flush();
  // // appDataSize = 1 + heltec_rs485_soil_sensor_data_length;
  // // appData[0] = batteryLevel;
  // // memcpy(appData + 1, heltec_rs485_soil_sensor_data_head, heltec_rs485_soil_sensor_data_length);
  // prepareData();
  lorawan_init();
}

void loop()
{
  get_battery_level();
  charge_power_control();
  lorawan_process();
  // storage_process();
}