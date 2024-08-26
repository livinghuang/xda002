#ifndef RS485_H
#define RS485_H
#include "global.h"
void rs485_init();
bool rs485_process(void);
bool fetch_data_process(void);
#endif