# Decoding Storage Architecture

This document illuminates the structure and organization of our data storage system, offering a detailed view on how we handle and arrange data.

## Directory and File Overview

- **`/board_info`**: A binary file dedicated to storing board-specific data, with a size of 256 bytes.
- **`/storage_info`**: Holds details on storage configurations, sized at 256 bytes in a binary format.
- **`/storage/pages/pageN`**: A series of files each limited to 256 bytes, designed to store data incrementally.

Directory Layout:

```md
├── /
├── /board_info
├── /storage_info
├── /storage/pages
│   ├── page0
│   ├── page1
│   ├── page2
│   ├── ...
│   └── pageN
```

## Storage Page Specifications

- **`EACH_PAGE_SIZE`**: The size set for each storage page, marked at 255 bytes.
- **`TOTAL_PAGES`**: The total quantity of pages, maxing out at UINT32_MAX for a 32-bit unsigned integer.
- **`EACH_LINE_SIZE`**: The byte size for each line within a page, tailored to match the size of the data it holds, ensuring each line represents a minimal data unit.
- **`EACH_PAGE_LINES`**: The total number of lines per page, calculated by dividing the page size by the line size.
- **`TOTAL_STORAGE_BYTES`**: The overall storage capacity in bytes, determined by multiplying the size of a page by the total number of pages.
- **`TOTAL_LINES`**: The total number of lines available across all storage, which equates to the total storage bytes, emphasizing that each line's size is directly proportional to a single unit of data, thereby aligning the total lines with the overall storage capacity.

## Detailed Storage Information and Operations

### Board Information (`/board_info`)

Stores crucial board data in a 256-byte binary file. It's accessible and manageable through `set_board_info()` and `get_board_info()` functions, facilitating the storage and retrieval of board-specific information.

### Storage Details (`/storage_info`)

Captures essential storage configurations in a 256-byte binary file. Operations such as `set_storage_info()` and `get_storage_info()` allow for the modification and acquisition of storage details.

### Data Pages (`/storage/pages/pageN`)

These files, each with a 256-byte capacity, store data efficiently. Functions like `write_page()` and `read_page()` enable data writing and reading operations for individual pages, ensuring data management is streamlined and effective.

## Storage Operation Methods

To facilitate various storage operations, a generic operation method is employed, typically executed within a loop to handle continuous data processing tasks. This method integrates seamlessly with the storage structure, providing a robust mechanism for data management.

### Operations Overview

The storage system is equipped with a range of operations designed to manage and interact with the storage architecture effectively. Each operation plays a crucial role in maintaining the integrity, accessibility, and efficiency of data storage and retrieval processes.

- **`format_storage()`**: This function initializes the storage with a File Allocation Table (FAT) filesystem. It is crucial to note that after formatting, a system restart is necessary to apply changes and ensure stability.

- **`save_to_storage(void *data)`**: Allows for the saving of data to the storage system. This function accepts a pointer to the data intended for storage, ensuring flexibility in handling various data types and structures.

- **`clr_record()`**: Clears or deletes all existing records within the storage, effectively resetting the data space. This operation is essential for maintaining storage hygiene and managing space efficiently.

- **`read_record()`**: Facilitates the reading of records from the storage. This function is pivotal for data retrieval, enabling the access and review of stored information.

- **`storage_init()`**: Initializes the storage system, preparing it for operation. This foundational step is critical for ensuring that the storage is ready to receive, store, and manage data effectively.

- **`storage_process()`**: Acts as the core loop for storage operations, necessitating its invocation within a continuous loop to perform ongoing storage tasks. This function orchestrates the execution of storage operations, maintaining active management of the storage system.

- **`get_storage_info()`**: Provides a comprehensive overview of storage metrics, including total space, free space, and detailed information on stored files. This operation is invaluable for monitoring and managing storage capacity and usage.

- **`read_record_init()`**: Initializes the mechanism for reading storage records, setting the stage for efficient data retrieval processes. This preparatory step ensures that the system is primed for accessing stored data accurately and swiftly.

Together, these operations constitute a robust framework for managing the storage architecture, ensuring data is stored, accessed, and managed efficiently and effectively.