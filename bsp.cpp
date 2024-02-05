#include "global.h"

board_information_union board_information;

extern uint32_t appTxDutyCycle; // unit: ms

void set_default_board_information(void)
{
  board_information.data.appTxDutyCycle = appTxDutyCycle;

  writeBinFile(FFat, "/board_information", &board_information, sizeof(board_information));

  Serial.println("Set default board information success");
  Serial.flush();
  delay(100);
}

void print_board_information(void)
{
  Serial.print("appTxDutyCycle:");
  Serial.println(board_information.data.appTxDutyCycle);
}

void board_init()
{
  if (storage_init())
  {
    Serial.println("Storage init success");
    if (readBinFile(FFat, "/board_information", &board_information, sizeof(board_information)))
    {
      Serial.println("read board information success:");
      print_board_information();
      set_default_board_information();
    }
    else
    {
      Serial.println("read board information failure: I am a new board");
      set_default_board_information();
    }
  }
  else
  {
    Serial.println("Storage init failed");
    format_storage();
  }
}