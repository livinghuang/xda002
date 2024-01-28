#include "GLOBAL.h"
#ifdef USE_AT
#include "AT.h"
#include "Arduino.h"

#define AT_BUFFER 256
extern enum Storage_Operation storage_operation;
const String ok_string = "OK";
const String error_string = "ERROR";
const String Invalid_params_string = "Invalid params string";
void command_error_handler(void);
bool command_executed;
void parseAndExecuteCommand(const char *command);
char at_buffer[AT_BUFFER];
bool is_param_a_string(const char *ptr, const int maxSize);
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
#ifdef AT_LRAINIT
    {"AT+LRAINIT", at_lrainit},
#endif
#ifdef AT_LRASND
    {"AT+LRASND", at_lrasnd},
#endif
#ifdef AT_BLEINIT
    {"AT+BLEINIT", at_bleinit},
#endif
#ifdef AT_BLESND
    {"AT+BLESND", at_blesnd},
#endif
#ifdef AT_BLESTOP
    {"AT+BLESTOP", at_blestop},
#endif
};

void at_start(void)
{
    Serial.begin(115200);
    Serial.print("CMD>");
    Serial.setTimeout(5000);
    bool command_executed = false;
}
void at_reset(void)
{
    memset(at_buffer, 0, AT_BUFFER);
}
void at_stop(void)
{
    Serial.end();
}

void at_process()
{
    if (Serial.available())
    {
        // Read a string terminated by a newline character '\n'
        String inputString = Serial.readStringUntil('\n');

        // Check if the read was successful
        if (inputString.length() > 0)
        {
            inputString.toCharArray(at_buffer, sizeof(at_buffer));
            parseAndExecuteCommand(at_buffer);
        }
        else
        {
            // Handle the timeout (no data received within the timeout period)
            Serial.println("Timeout occurred. No data received.");
        }
    }

    if (command_executed)
    {
        command_executed = false;
        at_reset();
        at_start();
    }
    vTaskDelay(1);
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
        command_executed = true;
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
    command_executed = true;
}

#ifdef AT
void at(const char *params)
{
    Serial.println("AT OK");
    if (deviceConnected)
    {
        send_to_ble("AT OK");
    }
}
#endif
#ifdef AT_CLRMEM
void at_clrmem(const char *params)
{
    storage_operation = _CLR;
    Serial.println(ok_string);
    command_executed = true;
    return;
}
#endif

#ifdef AT_READ
void at_read(const char *params)
{
    char *endptr;
    if ((params == NULL) || (!is_param_a_string(params, 10)))
    {
        // 无效的命令格式，可以进行错误处理
        Serial.println(error_string);
        command_executed = true;
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
        Serial.printf("readQty:%ld\n", readQty);
        storage_operation = _READ;
        read_record_init();
    }

    Serial.println(ok_string);
    command_executed = true;
    return;
}
#endif

#ifdef AT_WRITE
void at_write(const char *params)
{
    char *endptr;

    if ((params == NULL) || (!is_param_a_string(params, 10)))
    {
        // 无效的命令格式，可以进行错误处理
        Serial.println(error_string);
        command_executed = true;
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
        Serial.printf("writeData:%d\n", writeData);
        storage_operation = _WRITE;
        extern char storage_buffer[];
        memset(storage_buffer, 0, 4);
        memcpy(storage_buffer, &writeData, 2);
    }
    Serial.println(ok_string);
    command_executed = true;
}
#endif

#ifdef AT_FORMAT
void at_format(const char *params)
{
    storage_operation = _FORMAT;
    Serial.println(ok_string);
    command_executed = true;
}
#endif

#ifdef AT_INFO
bool at_info_flag = false;
void at_info(const char *params)
{
    storage_operation = _INFO;
    Serial.println(ok_string);
    command_executed = true;
}
#endif

#ifdef AT_FSHSTOP
void at_fshstop(const char *params)
{
    Serial.println(ok_string);
    command_executed = true;
    return;
}
#endif

#ifdef AT_LRAINIT
void at_lrainit(const char *params)
{
    bool result = true;

    if (result)
    {
        Serial.println(ok_string);
    }
    else
    {
        Serial.println(error_string);
    }
    command_executed = true;
    return;
}
#endif

#ifdef AT_LRASND
void at_lrasnd(const char *params)
{
    bool result = true;

    send_to_lora((uint8_t *)params);

    if (result)
    {
        Serial.println(ok_string);
    }
    else
    {
        Serial.println(error_string);
    }
    command_executed = true;
    return;
}
#endif

#ifdef AT_LRASTOP
void at_lrastop(const char *params)
{
    bool result = true;

    lora_stop();

    if (result)
    {
        Serial.println(ok_string);
    }
    else
    {
        Serial.println(error_string);
    }
    command_executed = true;
    return;
}
#endif

#ifdef AT_BLEINIT
void at_bleinit(const char *params)
{
    bool result = true;
    ble_init();
    if (result)
    {
        Serial.println(ok_string);
    }
    else
    {
        Serial.println(error_string);
    }
    command_executed = true;
    return;
}
#endif

#ifdef AT_BLESND
void at_blesnd(const char *params)
{
    bool result = true;

    char buffer[30];
    strcpy(buffer, params);
    send_to_ble(buffer);

    if (result)
    {
        Serial.println(ok_string);
    }
    else
    {
        Serial.println(error_string);
    }
    command_executed = true;
    return;
}
#endif

#ifdef AT_BLESTOP
void at_blestop(const char *params)
{
    bool result = true;

    ble_stop();

    if (result)
    {
        Serial.println(ok_string);
    }
    else
    {
        Serial.println(error_string);
    }
    command_executed = true;
    return;
}
#endif

void command_error_handler(void)
{
    Serial.println(Invalid_params_string);
    Serial.println(error_string);
    command_executed = true;
}

bool is_param_a_string(const char *ptr, const int maxSize)
{
    int i = 0; // 用于计数已检查的字节数

    while (i < maxSize)
    {
        if (ptr[i] == '\0')
        {
            return true;
        }
        i++;
    }
    return false;
}

#endif
