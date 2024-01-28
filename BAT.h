#include "GLOBAL.h"
#ifdef USE_BAT
#ifndef BAT_H
#define BAT_H

#include "Arduino.h"
#define BAT_ADC_PIN 2
float getBatteryVoltage();
uint8_t getBatteryLevel(float voltage);
void test_bat(void);
#endif
#endif
