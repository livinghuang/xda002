/*==============================================================

==============================================================*/

#ifndef STORAGE_H
#define STORAGE_H

enum Storage_Operation
{
  _INIT,    // mount FFAT and show the total space and free space
  _INFO,    // show the total space and free space and also show the storage files information
  _WRITE,   // write data
  _READ,    // read data
  _CLR,     // clear data
  _FORMAT,  // format FFAT
  _SET_BRD, // set board information
  _NONE     // do nothing, in standby
};

extern enum Storage_Operation storage_operation;

typedef struct storage_info_struct
{
  uint32_t page;
  int line;
  int oldest_page;
};

bool storage_init(void);    // it will execute in setup
void storage_process(void); // it is a loop for storage, you have to call it in loop to execute storage_operation

bool format_storage(void);        // format FFAT, user need restart the system
bool save_to_storage(void *data); // save data to storage
bool clr_record(void);            // clear storage record
void read_record(void);           // read storage record
void get_storage_info(void);

void read_record_init(void); // init the storage record

void set_board_info(void);   // set the board information
void get_board_info(void);   // show the board information
void set_storage_info(void); // set the storage information
void get_storage_info(void); // show the total space and free space and also show the storage files information
#endif