
#ifndef AT_H
#define AT_H
#include "global.h"
#define AT
// #define   AT_BOARD
// #define   AT_CALIB
// #define   AT_TEMPOFFSET
// #define AT_UPINTV
#define AT_FORMAT
#define AT_CLRMEM
#define AT_READ
#define AT_WRITE
#define AT_INFO
#define AT_FSHSTOP

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

#endif