#include "global.h"

#define MAIN_BAT_V_MIN 6.0 // Define the minimum voltage
#define MAIN_BAT_V_MAX 7.0 // Define the maximum voltage

void get_battery_level()
{
  float bat_voltage = getMainBatteryVoltage();
  Serial.printf("bat_voltage: %f\n", bat_voltage);
  Serial.flush();
  // Map the voltage to the 0-255 range
  sensor_data.main_battery_level = mapVoltageToLevel(bat_voltage, MAIN_BAT_V_MIN, MAIN_BAT_V_MAX);
  Serial.printf("main_battery_level: %d\n", sensor_data.main_battery_level);
  // delay(100);
}

byte mapVoltageToLevel(float voltage, float V_min, float V_max)
{
  if (voltage < V_min)
  {
    voltage = V_min;
  }
  else if (voltage > V_max)
  {
    voltage = V_max;
  }

  // Map the voltage to a 0-255 level
  return (byte)((voltage - V_min) * 255.0 / (V_max - V_min));
}

float getMainBatteryVoltage()
{
  uint32_t sum = 0;
  uint32_t adc = 0;
  pinMode(pM_VBAT_ADC, ANALOG);
  analogSetPinAttenuation(pM_VBAT_ADC, ADC_11db);
  delay(100);
  for (size_t i = 0; i < 8; i++)
  {
    adc = analogRead(pM_VBAT_ADC);
    sum += adc;
    // Serial.print("adc:");
    // Serial.println(adc);
    // Serial.flush();
  }
  float avg = sum / 8.0; // 4095 * 2500;
  // Serial.print("adc avg:");
  // Serial.println(avg);
  // Serial.flush();
  // delay(10);
  float voltage = avg / 4096 * 3.155;
  // Serial.print("adc volt:");
  // Serial.println(voltage);
  // Serial.flush();
  // delay(10);

  float bat_voltage = voltage * 1220 / 220 + 0.3;
  // Serial.print("battery volt:");
  // Serial.println(bat_voltage);
  // Serial.flush();
  // delay(1000);
  return bat_voltage;
}
