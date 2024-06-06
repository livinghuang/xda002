#include "global.h"

bool storage_init(void)
{
  bool fileExists = fileExistsInDir(FFat, "/", "board_info");
  if (fileExists)
  {
    // The Board info file was found in the root directory
    Serial.println("Board info file exists");
    // get_board_info();
  }
  else
  {
    // The Board info file was not found in the root directory
    Serial.println("Board info file does not exist");
    // set_board_info();
  }
}
