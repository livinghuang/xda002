#ifndef SOIL_H
#define SOIL_H
#include "global.h"

union INT_16
{
  int16_t value; // 用于直接存取组合后的值
  struct
  {
    uint8_t high;
    uint8_t low;
  };
};

void decode_soil(byte *data, uint8_t length);
#endif