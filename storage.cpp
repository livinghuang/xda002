// This file should be compiled with 'Partition Scheme' (in Tools menu)
// set to 'Default with ffat' if you have a 4MB ESP32 dev module or
// set to '16M Fat' if you have a 16MB ESP32 dev module.
#include "global.h"

#define STORAGE_INFO_FILE_NAME "/storage_info"

#define PAGE_NAME_HEAD "/page"

#define EACH_PAGE_SIZE 160     // UNIT: BYTES
#define TOTAL_PAGES UINT32_MAX // UNIT: EACH
#define MAX_FILE_NAME_LEN 100

#define EACH_LINE_SIZE DATA_LEN                            // UNIT: BYTES
#define EACH_PAGE_LINES (EACH_PAGE_SIZE / EACH_LINE_SIZE)  // UNIT: EACH
#define TOTAL_STORAGE_BYTES (EACH_PAGE_SIZE * TOTAL_PAGES) // UNIT: BYTES
#define TOTAL_LINES (TOTAL_STORAGE_BYTES / EACH_LINE_SIZE) // UNIT: EACH
#define RESERVE_FREE_SPACE EACH_PAGE_SIZE

enum Storage_Operation storage_operation = _NONE;
char storage_info_file_name[30] = {0};
storage_info_struct storage_info;
// show the total space and free space and also show the storage files information
void get_storage_info(void)
{
  readBinFile(FFat, STORAGE_INFO_FILE_NAME, &storage_info, sizeof(storage_info));
  print_storage_information();
}

void delete_oldest_file(void)
{
  char oldest_file_name[MAX_FILE_NAME_LEN] = {0};
  sprintf(oldest_file_name, "%s%d", PAGE_NAME_HEAD, storage_info.oldest_page);
  deleteFile(FFat, oldest_file_name);
  storage_info.oldest_page++;
  writeBinFile(FFat, STORAGE_INFO_FILE_NAME, &storage_info, sizeof(storage_info));
}
/*==============================================================
SAVE TO STORAGE
Here is the operation of saving data to storage files
each file has many page and each page has many lines
you could define the size of each line in EACH_LINE_SIZE
you could define the number of lines in EACH_PAGE_LINES
you could define the number of pages in TOTAL_PAGES
you could call EACH LINE as a data record
if storage files are full, delete the oldest one first
==============================================================*/
char storage_buffer[256] = {0};
bool save_to_storage(void *data)
{
  if (FFat.freeBytes() < RESERVE_FREE_SPACE)
  {
    delete_oldest_file();
  }
  char storage_file_name[MAX_FILE_NAME_LEN] = {0};
  sprintf(storage_file_name, "%s%d", PAGE_NAME_HEAD, storage_info.page);
  Serial.printf("save_to %s, line: %d\n", storage_file_name, storage_info.line);

  appendBinFile(FFat, storage_file_name, (char *)data, EACH_LINE_SIZE);
  storage_info.line++;
  if (storage_info.line >= EACH_PAGE_LINES)
  {
    storage_info.line = 0;
    storage_info.page++;
    if (storage_info.page > TOTAL_PAGES)
    {
      clr_record();
    }
  }
  Serial.print("next storage_info page");
  Serial.println(storage_info.page);
  Serial.print("next storage_info line");
  Serial.println(storage_info.line);
  return writeBinFile(FFat, STORAGE_INFO_FILE_NAME, &storage_info, sizeof(storage_info));
}
String data_read_buffer;
int32_t data_read_file(int file_number, int32_t rest_bytes)
{
  uint8_t buffer[EACH_PAGE_SIZE] = {0};
  char data_read_file_name[MAX_FILE_NAME_LEN];
  sprintf(data_read_file_name, "%s%d", PAGE_NAME_HEAD, file_number);
  File file = FFat.open(data_read_file_name, FILE_READ);
  size_t file_size = file.size();

  bool result = readBinFile(FFat, data_read_file_name, buffer, file_size);
  int total_lines_in_this_page = file_size / EACH_LINE_SIZE;
  int buffer_address = 0;

  buffer_address = (total_lines_in_this_page - 1) * EACH_LINE_SIZE;
  while (buffer_address >= 0)
  {
    for (size_t i = 0; i < EACH_LINE_SIZE; i++)
    {
      // Convert the byte to a two-character hexadecimal string
      String hexValue = String(buffer[buffer_address + i], HEX);

      // Ensure that the string has two characters (e.g., '0A' instead of 'A')
      if (hexValue.length() == 1)
      {
        hexValue = "0" + hexValue;
      }

      // Serial.print(hexValue);
      data_read_buffer = data_read_buffer + hexValue;
    }
    rest_bytes -= EACH_LINE_SIZE;
    if (rest_bytes <= 0)
    {
      return rest_bytes;
    }
    buffer_address = buffer_address - EACH_LINE_SIZE;
  }
  return rest_bytes;
}

long readQty = 0;
static bool read_end;
static int read_page;
static int32_t rest_bytes;
void read_record_init(void)
{
  read_end = false;
  read_page = storage_info.page;
  if (readQty != 0)
  {
    rest_bytes = readQty * EACH_LINE_SIZE;
  }
  else
  {
    rest_bytes = (storage_info.page - storage_info.oldest_page + 1) * EACH_PAGE_SIZE;
  }
}

void read_record(void)
{
  const char *buffer;
  if (read_page >= storage_info.oldest_page)
  {
    data_read_buffer = "";
    rest_bytes = data_read_file(read_page, rest_bytes);
    read_page--;
    if ((rest_bytes <= 0))
    {
      Serial.print(data_read_buffer);
      if (deviceConnected)
      {
        buffer = data_read_buffer.c_str();
        send_to_ble(buffer);
      }

      read_end = true;
      return;
    }
    Serial.print(data_read_buffer);
    if (deviceConnected)
    {
      buffer = data_read_buffer.c_str();
      send_to_ble(buffer);
    }
  }
  else
  {
    read_end = true;
  }
}

// Begin FFAT initialization and show the total space and free space
bool storage_init(void)
{
  if (!FFat.begin())
  {
    FFat.end();
    return false;
  }
  delay(100);
  Serial.println("\n====================");
  Serial.printf("Total space: %10u\n", FFat.totalBytes());
  Serial.printf("Free space: %10u\n", FFat.freeBytes());
  Serial.println("====================");
  Serial.flush();
  listDir(FFat, "/", 3);
  bool fileExists = fileExistsInDir(FFat, "/", "board_info");

  if (fileExists)
  {
    // The Board info file was found in the root directory
    Serial.println("Board info file exists");
    get_board_info();
  }
  else
  {
    // The Board info file was not found in the root directory
    Serial.println("Board info file does not exist");
    set_board_info();
  }

  fileExists = fileExistsInDir(FFat, "/", "storage_info");
  if (fileExists)
  {
    // The file storage info file was found in the root directory
    Serial.println("File exists");
    get_storage_info();
  }
  else
  {
    // The file STORAGE_INFO_FILE was not found in the root directory
    Serial.println("File does not exist");
    clr_record();
  }
  return true;
}

bool format_storage(void)
{
  FFat.end();
  FFat.format();
  Serial.println("FFAT Formatted,please restart system");
  Serial.flush();
}
bool clr_record(void)
{
  char delete_file_name[MAX_FILE_NAME_LEN];
  int page = storage_info.page;
  while (page >= 0)
  {
    sprintf(delete_file_name, "%s%d", PAGE_NAME_HEAD, page);
    deleteFile(FFat, delete_file_name);
    page--;
  }
  storage_info.line = 0;
  storage_info.page = 0;
  storage_info.oldest_page = 0;
  return writeBinFile(FFat, STORAGE_INFO_FILE_NAME, &storage_info, sizeof(storage_info));
}

void set_board_info(void)
{
  board_information.data.appTxDutyCycle = appTxDutyCycle;

  writeBinFile(FFat, "/board_info", &board_information, sizeof(board_information));

  Serial.println("Set default board information success");
  Serial.flush();
}

void get_board_info(void)
{
  if (readBinFile(FFat, "/board_info", &board_information, sizeof(board_information)))
  {
    Serial.println("Read board information success");
    print_board_information();
  }
  else
  {
    Serial.println("Read board information failure: I am a new board! Now I will set board information as default");
    set_board_info();
  }
}
void set_storage_info(void)
{
}

void storage_process(void)
{
  uint8_t buffer[256] = {0};

  switch (storage_operation)
  {
  case _INIT:
    storage_init();
    storage_operation = _NONE;
    break;
  case _INFO:
    get_board_info();
    get_storage_info();
    if (deviceConnected)
    {
      strcpy(ble_buffer, board_information_string);
      strcat(ble_buffer, storage_information_string);

      send_to_ble(ble_buffer);
    }
    storage_operation = _NONE;
    break;
  case _WRITE:
    save_to_storage(storage_buffer);
    storage_operation = _NONE;
    break;
  case _READ:
    if (!read_end)
    {
      read_record();
    }
    else
    {
      storage_operation = _NONE;
    }
    break;
  case _CLR:
    clr_record();
    storage_operation = _NONE;
    break;
  case _FORMAT:
    format_storage();
    storage_operation = _NONE;
    break;
  case _SET_BRD:
    set_board_info();
    storage_operation = _NONE;
    break;
  case _NONE:
    break;
  }
}
