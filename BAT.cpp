#include "GLOBAL.h"
#ifdef USE_BAT
#include "BAT.h"
#include "Arduino.h"
static int analogVolts = 0;

float getBatteryVoltage(void)
{
    // Set the resolution to 12 bits (0-4096)
    analogReadResolution(12);

    // Read the analog / millivolts value for pin 2:

    uint8_t i = 50;

    long sum = 0;
    while (i)
    {
        analogVolts = analogReadMilliVolts(BAT_ADC_PIN);
        sum += analogVolts;
        i--;
    }
    sum = sum / 50;
    analogVolts = sum;
    // Convert ADC reading to battery voltage
    float batteryVoltage = analogVolts * (510.0 + 220.0) / 220.0 / 1000;

    return batteryVoltage;
}
uint8_t getBatteryLevel(float voltage)
{
    uint8_t level;
    if (voltage > 4.5)
    {
        level = 255; // Battery voltage is over 4.5V
    }
    else if (voltage >= 4.2)
    {
        level = 254; // Battery voltage is between 4.2V and 4.5V
    }
    else if (voltage >= 3.0)
    {
        level = (voltage - 3.0) / (4.2 - 3.0) * 253;
        // Battery voltage is between 3.0V and 4.2V
    }
    else
    {
        level = 0; // Battery voltage is below 3.0V
    }
    return level;
}
void test_bat(void)
{
    Serial.printf("bat:%f\n", getBatteryVoltage());
    Serial.flush();
}
#endif