#include "global.h"

#define AT_BUFFER 256
const char ok_string[] = "OK\r\n";
const char error_string[] = "ERROR\r\n";
const char Invalid_params_string[] = "Invalid params string\r\n";
void command_error_handler(void);
void parseAndExecuteCommand(const char *command);
char at_buffer[AT_BUFFER];
typedef void (*CommandHandler)(const char *params);

// 命令列表定义
typedef struct
{
  const char *name;
  CommandHandler handler;
} Command;
Command commandList[] = {
#ifdef AT
    {"AT", at},
#endif
#ifdef AT_NAME
    {"AT+NAME", at_name},
#endif
#ifdef AT_BOARD
    {"AT+BOARD", at_board},
#endif
#ifdef AT_CALIB
    {"AT+CALIB", at_calib},
#endif
#ifdef AT_TEMPOFFSET
    {"AT+TEMPOFFSET", at_tempoffset},
#endif
#ifdef AT_UPINTV
    {"AT+UPINTV", at_upintv},
#endif
#ifdef AT_FORMAT
    {"AT+FORMAT", at_format},
#endif
#ifdef AT_CLRMEM
    {"AT+CLRMEM", at_clrmem},
#endif
#ifdef AT_READ
    {"AT+READ", at_read},
#endif
#ifdef AT_WRITE
    {"AT+WRITE", at_write},
#endif
#ifdef AT_INFO
    {"AT+INFO", at_info},
#endif
};

void at_start(void)
{
  Serial.begin(115200);
  Serial.print("CMD>");
  Serial.setTimeout(5000);
}
void at_reset(void)
{
  memset(at_buffer, 0, AT_BUFFER);
}
void at_stop(void)
{
  Serial.end();
}

// 解析和执行命令
void parseAndExecuteCommand(const char *command)
{
  size_t i = 0;
  char buffer[128];
  memcpy(buffer, command, 128);

  // 解析命令名称和参数
  char *token = (char *)strtok((char *)buffer, "=");

  if (token == NULL)
  {
    // 无效的命令格式，可以进行错误处理
    Serial.println(error_string);
    return;
  }

  // 查找命令处理函数
  for (i = 0; i < sizeof(commandList) / sizeof(commandList[0]); i++)
  {
    if (strcmp(token, commandList[i].name) == 0)
    {
      // 找到匹配的命令处理函数，提取参数并执行
      token = (char *)strtok(NULL, ",");
      commandList[i].handler(token);
      return;
    }
  }
  // 未找到匹配的命令处理函数，可以进行错误处理
  Serial.println(error_string);
}

#ifdef AT
void at(const char *params)
{
  static char answer[] = "AT OK\0";
  extern char txValueBuffer[];
  strcpy(txValueBuffer, answer);
  Serial.println(txValueBuffer);
  ble_send_data_ready();
}
#endif

#ifdef AT_NAME
void at_name(const char *params)
{
  char buffer[10];
  extern char txValueBuffer[];
  char *endptr;
  if ((params == NULL))
  {
    // 无效的命令格式，可以进行错误处理
    Serial.println(error_string);
    return;
  }
  strncpy(buffer, params, 10);
  buffer[10] = '\0';
  strcpy(txValueBuffer, buffer);
  Serial.println(txValueBuffer);
  writeFile(FFat, "/name.txt", buffer);
  readName(FFat);
  ble_send_data_ready();
}
#endif

#ifdef AT_CLRMEM

void at_clrmem(const char *params)
{
  storage_operation = _CLR;
  Serial.println(ok_string);
  return;
}
#endif

#ifdef AT_READ
void at_read(const char *params)
{
  char *endptr;
  if ((params == NULL) || (!is_a_string(params, 10)))
  {
    // 无效的命令格式，可以进行错误处理
    Serial.println(error_string);
    return;
  }
  extern long readQty;
  readQty = strtol(params, &endptr, 10);

  if (*endptr != '\0' && *endptr != '\n')
  {
    command_error_handler();
  }
  else
  {
    char buffer[128];
    sprintf(buffer, "readQty:%ld\r\n", readQty);
    Serial.println(buffer);
    storage_operation = _READ;
  }

  Serial.println(ok_string);
  return;
}
#endif

#ifdef AT_WRITE
void at_write(const char *params)
{
  char *endptr;

  if ((params == NULL) || (!is_a_string(params, 10)))
  {
    // 无效的命令格式，可以进行错误处理
    Serial.println(error_string);
    return;
  }

  long data = strtol(params, &endptr, 10);

  if (*endptr != '\0' && *endptr != '\n')
  {
    command_error_handler();
  }
  else
  {
    uint16_t writeData = static_cast<uint16_t>(data);
    // Serial.printf("writeData:%d\n", writeData);
    char buffer[128];
    sprintf(buffer, "writeData:%d\r\n", writeData);
    Serial.println(buffer);
    extern char storage_buffer[];
    memset(storage_buffer, 0, 4);
    memcpy(storage_buffer, &writeData, 2);
    storage_operation = _WRITE;
  }
  Serial.println(ok_string);
}
#endif

#ifdef AT_FORMAT
void at_format(const char *params)
{
  extern char txValueBuffer[];
  if (FFat.format())
  {
    strncpy(txValueBuffer, "OK", 3);
  }
  else
  {
    strncpy(txValueBuffer, "ERROR", 6);
  }
  Serial.println(txValueBuffer);
  ble_send_data_ready();
}
#endif

#ifdef AT_UPINTV
void at_upintv(const char *params)
{
  char *endptr;
  if ((params == NULL) || (!is_a_string(params, 10)))
  {
    // 无效的命令格式，可以进行错误处理
    Serial.println(error_string);
    return;
  }

  if (*params == '?')
  {
    extern char txValueBuffer[];
    snprintf(txValueBuffer, 50, "%lu ,OK", appTxDutyCycle);
    Serial.println(txValueBuffer);
  }
  else
  {
    long data = strtol(params, &endptr, 10);
    appTxDutyCycle = data;
    Serial.flush();

    extern char txValueBuffer[];
    snprintf(txValueBuffer, 50, "%lu ,OK", appTxDutyCycle);
    Serial.println(txValueBuffer);
    char buffer[10];
    ltoa(appTxDutyCycle, buffer, 10);
    writeFile(FFat, "/upintv.txt", buffer);
    readUpIntv(FFat);
  }
  ble_send_data_ready();
}
#endif

#ifdef AT_INFO
bool at_info_flag = false;
void at_info(const char *params)
{
  storage_operation = _INFO;
  Serial.println(ok_string);
}
#endif

void command_error_handler(void)
{
  Serial.println(Invalid_params_string);
  Serial.println(error_string);
}
