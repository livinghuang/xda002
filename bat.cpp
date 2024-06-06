#include "global.h"

#define SUB_BAT_EMPTY 3.3
#define SUB_BAT_FULL 4.1

#define MAIN_BAT_V_MIN 11.5 // Define the minimum voltage
#define MAIN_BAT_V_MAX 13.5 // Define the maximum voltage

void get_battery_level()
{
  float bat_voltage = getMainBatteryVoltage();
  Serial.print("bat_voltage: ");
  Serial.println(bat_voltage);
  delay(1000);
  // Map the voltage to the 0-255 range
  sensor_data.battery_level = mapVoltageToLevel(bat_voltage, MAIN_BAT_V_MIN, MAIN_BAT_V_MAX);
  Serial.print("battery_level: ");
  Serial.println(sensor_data.battery_level);
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

void charge_power_path_on()
{
  pinMode(pPOW_CHG, OUTPUT);
  digitalWrite(pPOW_CHG, LOW);
  Serial.println("Charge Power On");
  Serial.flush();
  delay(10);
}

void charge_power_path_off()
{
  pinMode(pPOW_CHG, OUTPUT);
  digitalWrite(pPOW_CHG, HIGH);
  Serial.println("Charge Power Off");
  Serial.flush();
  delay(10);
}

void charge_power_control()
{
  float bat_voltage = getSubBatteryVoltage();
  if (bat_voltage < SUB_BAT_EMPTY)
  {
    charge_power_path_on();
  }
  if (bat_voltage > SUB_BAT_FULL)
  {
    charge_power_path_off();
  }
}

float getMainBatteryVoltage()
{
  uint32_t sum = 0;
  uint32_t adc = 0;
  analogSetPinAttenuation(pM_VBAT_ADC, ADC_11db);
  pinMode(pVBAT_Ctrl, ANALOG);
  for (size_t i = 0; i < 16; i++)
  {
    adc = analogRead(pM_VBAT_ADC);
    sum += adc;
    // Serial.print("adc:");
    // Serial.println(adc);
    // Serial.flush();
    // delay(10);
  }
  float avg = sum / 16.0; // 4095 * 2500;
  // Serial.print("avg:");
  // Serial.println(avg);
  // Serial.flush();
  // delay(1000);
  float voltage = avg / 4095 * 2.97;
  // Serial.print("volt:");
  // Serial.println(voltage);
  // Serial.flush();
  // delay(1000);

  float bat_voltage = voltage * 1220 / 220;
  // Serial.print("battery volt:");
  // Serial.println(bat_voltage);
  // Serial.flush();
  // delay(1000);
  return bat_voltage;
}

float getSubBatteryVoltage()
{
  uint32_t sum = 0;
  uint32_t adc = 0;
  analogSetPinAttenuation(pS_VBAT_ADC, ADC_11db);
  pinMode(pVBAT_Ctrl, ANALOG);
  for (size_t i = 0; i < 16; i++)
  {
    adc = analogRead(pS_VBAT_ADC);
    sum += adc;
    // Serial.print("adc:");
    // Serial.println(adc);
    // Serial.flush();
    // delay(10);
  }
  float avg = sum / 16.0; // 4095 * 2500;
  // Serial.print("avg:");
  // Serial.println(avg);
  // Serial.flush();
  // delay(1000);
  float voltage = avg / 4095 * 2.97;
  // Serial.print("volt:");
  // Serial.println(voltage);
  // Serial.flush();
  // delay(1000);

  float bat_voltage = voltage * 1020 / 510;
  // Serial.print("battery volt:");
  // Serial.println(bat_voltage);
  // Serial.flush();
  // delay(1000);
  return bat_voltage;
}
