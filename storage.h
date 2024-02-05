/*==============================================================
Architecture Of Storage

# Storage Architecture Explanation

This document provides an explanation of the storage architecture defined in your code.

## File Names and Paths

- **`STORAGE_INFO_FILE_NAME`**: The name of the storage information file, which is set to "/storage_info".

- **`PAGE_NAME_HEAD`**: The head of the page name, set to "/page".

## Storage Page Configuration

- **`EACH_PAGE_SIZE`**: The size of each storage page in bytes, set to 160 bytes.

- **`TOTAL_PAGES`**: The total number of storage pages, set to UINT32_MAX (maximum unsigned 32-bit integer).

- **`EACH_LINE_SIZE`**: The size of each line within a page in bytes, set to 2 bytes.

- **`EACH_PAGE_LINES`**: The number of lines within each storage page, calculated as `EACH_PAGE_SIZE / EACH_LINE_SIZE`.

- **`TOTAL_STORAGE_BYTES`**: The total storage capacity in bytes, calculated as `EACH_PAGE_SIZE * TOTAL_PAGES`.

- **`TOTAL_LINES`**: The total number of lines that can be stored, calculated as `TOTAL_STORAGE_BYTES / EACH_LINE_SIZE`.

## Storage Information Structure

- **`struct storage_info_struct`**: A structure that holds storage information including:
  - `page`: The current page.
  - `line`: The current line within the page.
  - `oldest_page`: The oldest page in the storage.

## Storage Information File Name

- **`storage_info_file_name`**: An array used to store the name of the storage information file with a maximum length of 30 characters.
==============================================================*/

#ifndef STORAGE_H
#define STORAGE_H

bool format_storage(void);
bool save_to_storage(void *data);
bool clr_record(void);
void read_record(void);
bool storage_init(void);
void storage_process(void);
void get_storage_info(void);
void read_record_init(void);
#endif