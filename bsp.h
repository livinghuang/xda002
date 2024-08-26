#ifndef BSP_H
#define BSP_H

// #define pEXT_12_ctrl 0
#define pM_VBAT_ADC 1
#define pVext 2

#define pLORA_RST 5
// RS485
#define DI 19   // Driver Input (DI) share with SX1262 (MISO)
#define RO 18   // Receiver Output (RO) share with SX1262 (MOSI)
#define RE_DE 0 // Data Enable (DE) share with SX1262 (NSS, internal pull up)

// #define pLED 10 // LED Direct Drive LED with 120 resistor, share with SX1262 (SCK)

#define BOARD_INFORMATION_SIZE 256

typedef union board_information_union
{
  uint8_t whole[BOARD_INFORMATION_SIZE];
  struct
  {
    uint32_t appTxDutyCycle;
  } data;
};
extern board_information_union board_information;

void print_board_information(void);
void print_storage_information(void);
void board_init();
#endif