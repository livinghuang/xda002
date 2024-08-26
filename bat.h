#ifndef BAT_H
#define BAT_H
#include "global.h"
float getMainBatteryVoltage();
byte mapVoltageToLevel(float voltage, float V_min, float V_max);
void get_battery_level();
#endif