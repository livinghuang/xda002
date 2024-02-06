#ifndef BSP_H
#define BSP_H

#define pVext 1
#define pADC_BAT 2
#define pSDA 18
#define pSCL 19
#define pLORA_RST 5

// RS485
#define RO485_RO 19 // Receiver Output (RO) connected to pin 19
#define RO485_DI 18 // Driver Input (DI) connected to pin 18
#define RO485_DE 4  // Data Enable (DE) connected to pin 4

// 24 POWER
#define pSDN 3

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