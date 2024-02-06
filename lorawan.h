#ifndef LORA_H
#define LORA_H

#include "global.h"

#define LORA_RTS LORA_STA
#define LORA_CTS LORA_READY
void lorawan_init(void);
void lorawan_process(void);
void send_to_lora(uint8_t *data);
void generate_lorawan_parameters_by_chip_id(void);
#endif