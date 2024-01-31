#include "global.h"
#ifndef AT_H
#define AT_H

#define AT
// #define   AT_BOARD
// #define   AT_CALIB
// #define   AT_TEMPOFFSET
// #define   AT_UPINTV
#ifdef USE_FLASH
#define AT_FORMAT
#define AT_CLRMEM
#define AT_READ
#define AT_WRITE
#define AT_INFO
#define AT_FSHSTOP
#endif
#ifdef USE_LORA
#define AT_LRAINIT
#define AT_LRASND
#define AT_LRASTOP
#endif
#ifdef USE_BLE
#define AT_BLEINIT
#define AT_BLESND
#define AT_BLESTOP
#endif
extern bool command_executed;

void at_start(void);
void at_reset(void);
void at_stop(void);
void at_process();
// void L_printf(const char *data);
void parseAndExecuteCommand(const char *command);
// 命令处理函数定义
#ifdef AT
void at(const char *params);
#endif
#ifdef AT_NAME
void at_name(const char *params);
#endif
#ifdef AT_BOARD
void at_board(const char *params);
#endif
#ifdef AT_CALIB
void at_calib(const char *params);
#endif
#ifdef AT_TEMPOFFSET
void at_tempoffset(const char *params);
#endif
#ifdef AT_UPINTV
void at_upintv(const char *params);
#endif
#ifdef AT_FORMAT
void at_format(const char *params);
#endif
#ifdef AT_CLRMEM
void at_clrmem(const char *params);
#endif
#ifdef AT_READ
void at_read(const char *params);
#endif
#ifdef AT_WRITE
void at_write(const char *params);
#endif
#ifdef AT_INFO
void at_info(const char *params);
#endif
#ifdef AT_FSHSTOP
void at_fshstop(const char *params);
#endif

#ifdef AT_LRAINIT
void at_lrainit(const char *params);
#endif
#ifdef AT_LRASND
void at_lrasnd(const char *params);
#endif
#ifdef AT_LRASTOP
void at_lrastop(const char *params);
#endif

#ifdef AT_BLEINIT
void at_bleinit(const char *params);
#endif
#ifdef AT_BLESND
void at_blesnd(const char *params);
#endif
#ifdef AT_BLESTOP
void at_blestop(const char *params);
#endif

#endif