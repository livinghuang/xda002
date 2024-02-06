#include "global.h"

board_information_union board_information;
char ble_buffer[256] = {0};
extern uint32_t appTxDutyCycle; // unit: ms

char board_information_string[256] = {0};
char storage_information_string[256] = {0};
void print_board_information(void)
{
  sprintf(board_information_string, "appTxDutyCycle:%d\r\n", board_information.data.appTxDutyCycle);
  Serial.print(board_information_string);
}

void print_storage_information(void)
{
  sprintf(storage_information_string, "storage_info\r\n line:%d\r\n page:%d\r\n oldest page:%d\r\n", storage_info.line, storage_info.page, storage_info.oldest_page);
  Serial.print(storage_information_string);
}

void board_init()
{
  if (storage_init())
  {
    Serial.println("Storage init success");
  }
  else
  {
    Serial.println("Storage init failed");
    format_storage();
  }
}