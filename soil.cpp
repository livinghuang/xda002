
#include "global.h"

struct soil_data
{
  union INT_16 temperature;
  union INT_16 moisture;
  union INT_16 salt;
  union INT_16 conductivity;
  union INT_16 ph;
  union INT_16 nitrogen;
  union INT_16 phosphorus;
  union INT_16 potassium;
} soil_data;

void decode_soil(byte *data, uint8_t length)
{
  // char data[] = {0xFF, 0xDD, 0x01, 0x64, 0x04, 0xD2, 0x03, 0xF0, 0x02, 0xAE, 0x00, 0x87, 0x00, 0x8A, 0x00, 0x8E};
  // int length = sizeof(data) / sizeof(data[0]);

  // Rearrange the data
  for (int i = 0; i < length; i += 2)
  {
    char temp;
    temp = data[i + 1];
    data[i + 1] = data[i];
    data[i] = temp;
  }
  memcpy(&soil_data, data, sizeof(soil_data));
  Serial.printf("Temperature: %.1f°C\n", (double)soil_data.temperature.value / 10);
  Serial.printf("moisture: %.1f %\n", (double)soil_data.moisture.value / 10);
  Serial.printf("salt: %d uS/cm\n", soil_data.salt.value);
  Serial.printf("conductivity: %d uS/cm\n", soil_data.conductivity.value);
  Serial.printf("ph: %.2f \n", (double)soil_data.ph.value / 100);
  Serial.printf("nitrogen: %d mg/kg\n", soil_data.nitrogen.value);
  Serial.printf("phosphorus: %d mg/kg\n", soil_data.phosphorus.value);
  Serial.printf("potassium: %d mg/kg\n", soil_data.potassium.value);
}
