#ifndef BAT_H
#define BAT_H
#include "global.h"
float getMainBatteryVoltage();
float getSubBatteryVoltage();
void charge_power_control();
byte mapVoltageToLevel(float voltage, float V_min, float V_max);
void get_battery_level();
#endif