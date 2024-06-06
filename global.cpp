#include "global.h"
// Dps310 Opject
bool resendflag = false;
bool deepsleepflag = false;
bool deviceConnected = false;
bool oldDeviceConnected = false;
sensor_data_t sensor_data;
RTC_DATA_ATTR int bootCount = 0;
uint64_t chipid;
void printHex(byte *data, int length)
{
  for (int i = 0; i < length; i++)
  {
    // Print each byte in hexadecimal format with leading zeros
    if (data[i] < 0x10)
    {
      Serial.print("0");
    }
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.println(); // Print a newline character at the end
}

// Default stop callback function
void default_stop_callback()
{
  printf("Default Stop Callback\n");
}
// Function that runs a loop with a time-based escape period and calls the callback function
// It also has a timeout condition and an optional stop_callback
bool reset_run_with_time_escape = false;
void run_with_time_escape(uint64_t escape_period_ms, void (*callback)(), void (*stop_callback)())
{
  if (stop_callback == NULL)
  {
    stop_callback = default_stop_callback; // Use the default stop callback if not provided
  }
  uint64_t start_time = esp_timer_get_time(); // Get system timer value in microseconds
  while (1)
  {
    uint64_t escaped_time = (esp_timer_get_time() - start_time) / 1000; // Get system timer value in microseconds
    if (escaped_time >= escape_period_ms)
    {
      stop_callback();
      // printf("run_with_time_escape done\n");
      break; // Exit the loop when the timeout is reached
    }
    else
    {
      callback(); // Call the callback function
    }
    // Other processing can go here
    if (reset_run_with_time_escape)
    {
      reset_run_with_time_escape = false;
      // printf("Resetting run_with_time_escape\n");
      start_time = esp_timer_get_time();
    }
    // Sleep for a short interval to avoid busy-waiting
    delay(10); // Sleep for 1 millisecond
  }
}

esp_sleep_wakeup_cause_t print_wakeup_reason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT0:
    Serial.println("Wakeup caused by external signal using RTC_IO");
    break;
  case ESP_SLEEP_WAKEUP_EXT1:
    Serial.println("Wakeup caused by external signal using RTC_CNTL");
    break;
  case ESP_SLEEP_WAKEUP_TIMER:
    Serial.println("Wakeup caused by timer");
    break;
  case ESP_SLEEP_WAKEUP_TOUCHPAD:
    Serial.println("Wakeup caused by touchpad");
    break;
  case ESP_SLEEP_WAKEUP_ULP:
    Serial.println("Wakeup caused by ULP program");
    break;
  default:
    Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
    break;
  }

  return wakeup_reason;
}

uint64_t get_chip_id()
{
  uint64_t chipid = ESP.getEfuseMac();
  Serial.printf("ESP32ChipID=%04X%08X\n", (uint16_t)(chipid >> 32), (uint32_t)chipid);
  return chipid;
}
bool is_a_string(const char *ptr, const int maxSize)
{
  int i = 0; // 用于计数已检查的字节数

  while (i < maxSize)
  {
    if (ptr[i] == '\0')
    {
      if (i == 0)
      {
        return false;
      }
      return true;
    }
    i++;
  }
  return false;
}

// Define the struct
typedef struct WaterMeterData
{
  // 總累積: Total Accumulation
  int64_t totalAccumulationValue;
  char totalAccumulationPoint;

  // 瞬間流量: Instantaneous Flow
  int64_t instantaneousFlowValue;
  char instantaneousFlowPoint;

  // 正向累積: Forward Accumulation
  int64_t forwardAccumulationValue;
  char forwardAccumulationPoint;

  // 反向累積: Reverse Accumulation
  int64_t reverseAccumulationValue;
  char reverseAccumulationPoint;

  // 漏水天數: Leak Days
  int16_t leakDays;

  // 負載天數: Load Days
  int16_t loadDays;

  // 靜止天數: No-flow Days
  int16_t noFlowDays;

  // 反向天數: Reverse Days
  int16_t reverseDays;

  // 受磁天數: Magnetic Influence Days
  int16_t magneticInfluenceDays;

  // 電力不足天數: Power Insufficiency Days
  int16_t powerInsufficiencyDays;
} WaterMeterData;

typedef struct WaterMeterData data;
void prepareData(void)
{
  // float batteryVoltage = getBatteryVoltage();
  // byte batteryLevel = getBatteryLevel();
  // uint8_t heltec_rs485_soil_sensor_data_length = responseBuffer[2];
  // uint8_t *heltec_rs485_soil_sensor_data_head = &responseBuffer[3];

  // decode_soil(heltec_rs485_soil_sensor_data_head, heltec_rs485_soil_sensor_data_length);
  // Serial.flush();
  // appDataSize = 1 + heltec_rs485_soil_sensor_data_length;
  // appData[0] = batteryLevel;
  // memcpy(appData + 1, heltec_rs485_soil_sensor_data_head, heltec_rs485_soil_sensor_data_length);

  WaterMeterData meterData = {
      .totalAccumulationValue = 1234567899,
      .totalAccumulationPoint = 2,
      .instantaneousFlowValue = -123456,
      .instantaneousFlowPoint = 4,
      .forwardAccumulationValue = 1234567899,
      .forwardAccumulationPoint = 6,
      .reverseAccumulationValue = -123456,
      .reverseAccumulationPoint = 3,
      .leakDays = 9,
      .loadDays = 10,
      .noFlowDays = 11,
      .reverseDays = 12,
      .magneticInfluenceDays = 13,
      .powerInsufficiencyDays = 14};

  appData[0] = 128;
  appData[1] = 196;
  appDataSize = 46;
  memcpy(&appData[2], &meterData.totalAccumulationValue, 8);
  appData[10] = meterData.totalAccumulationPoint;
  memcpy(&appData[11], &meterData.instantaneousFlowValue, 4);
  appData[15] = meterData.instantaneousFlowPoint;
  memcpy(&appData[16], &meterData.forwardAccumulationValue, 8);
  appData[24] = meterData.forwardAccumulationPoint;
  memcpy(&appData[25], &meterData.reverseAccumulationValue, 8);
  appData[33] = meterData.reverseAccumulationPoint;
  memcpy(&appData[34], &meterData.leakDays, 2);
  memcpy(&appData[36], &meterData.loadDays, 2);
  memcpy(&appData[38], &meterData.noFlowDays, 2);
  memcpy(&appData[40], &meterData.reverseDays, 2);
  memcpy(&appData[42], &meterData.magneticInfluenceDays, 2);
  memcpy(&appData[44], &meterData.powerInsufficiencyDays, 2);
  for (uint8_t i = 0; i < appDataSize; i++)
  {
    Serial.print(appData[i], HEX);
  }
  Serial.println();
  char buff[100];
  memcpy(buff, &meterData, appDataSize);
  for (uint8_t i = 0; i < appDataSize; i++)
  {
    Serial.print(buff[i], HEX);
  }
  Serial.println();
}
