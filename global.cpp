#include "global.h"
// Dps310 Opject
bool resendflag = false;
bool deepsleepflag = false;
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
    Serial.flush();
  }
  Serial.println(); // Print a newline character at the end
}

// Default stop callback function
void default_stop_callback()
{
  printf("Default Stop Callback\n");
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

uint32_t simpleHash(const char *str)
{
  uint32_t hash = 0;
  while (*str)
  {
    hash = (hash * 31) + *str++;
  }
  return hash;
}

void sleep_test(void)
{
  Serial.println("Sleep test start");
  // lorawan_init();
  LoRaWAN.cycle(30000);
  while (1)
  {
    LoRaWAN.sleep(loraWanClass);
  }
  Serial.println("Sleep test end");
}
void print_bytes(uint8_t *data, int length)
{
  for (int i = 0; i < length; i++)
  {
    if (data[i] < 0x10)
    {
      Serial.print("0");
    }
    Serial.print(data[i], HEX);
    // Serial.print(" ");
  }
  Serial.println();
}

void print_bytes_reverse(uint8_t *data, int length)
{
  for (int i = length - 1; i >= 0; i--)
  {
    if (data[i] < 0x10)
    {
      Serial.print("0");
    }
    Serial.print(data[i], HEX);
    // Serial.print(" ");
  }
  Serial.println();
}