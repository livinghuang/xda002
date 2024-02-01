#ifndef RS485_H
#define RS485_H
#include "global.h"
void rs485_init();
void rs485_process(void);
// uint8_t rs485_process(byte *responseBuffer, uint8_t fetch_times);
#endif