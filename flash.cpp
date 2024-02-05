// This file should be compiled with 'Partition Scheme' (in Tools menu)
// set to 'Default with ffat' if you have a 4MB ESP32 dev module or
// set to '16M Fat' if you have a 16MB ESP32 dev module.
#include "GLOBAL.h"

//==============================================================
// void readFile(fs::FS &fs, const char *path)
// {
//     Serial.printf("Reading file: %s\r\n", path);

//     File file = fs.open(path);
//     if (!file || file.isDirectory())
//     {
//         Serial.println("- failed to open file for reading");
//         return;
//     }

//     Serial.println("- read from file:");
//     while (file.available())
//     {
//         Serial.write(file.read());
//     }
//     file.close();
// }

// void writeFile(fs::FS &fs, const char *path, const char *message)
// {
//     Serial.printf("Writing file: %s\r\n", path);

//     File file = fs.open(path, FILE_WRITE);
//     if (!file)
//     {
//         Serial.println("- failed to open file for writing");
//         return;
//     }
//     if (file.print(message))
//     {
//         Serial.println("- file written");
//     }
//     else
//     {
//         Serial.println("- write failed");
//     }
//     file.close();
// }

// void appendFile(fs::FS &fs, const char *path, const char *message)
// {
//     Serial.printf("Appending to file: %s\r\n", path);

//     File file = fs.open(path, FILE_APPEND);
//     if (!file)
//     {
//         Serial.println("- failed to open file for appending");
//         return;
//     }
//     if (file.print(message))
//     {
//         Serial.println("- message appended");
//     }
//     else
//     {
//         Serial.println("- append failed");
//     }
//     file.close();
// }

// void renameFile(fs::FS &fs, const char *path1, const char *path2)
// {
//     Serial.printf("Renaming file %s to %s\r\n", path1, path2);
//     if (fs.rename(path1, path2))
//     {
//         Serial.println("- file renamed");
//     }
//     else
//     {
//         Serial.println("- rename failed");
//     }
// }

void deleteFile(fs::FS &fs, const char *path)
{
  Serial.printf("Deleting file: %s\r\n", path);
  if (fs.remove(path))
  {
    Serial.println("- file deleted");
  }
  else
  {
    Serial.println("- delete failed");
  }
}

//==============================================================

bool fileExistsInDir(fs::FS &fs, const char *dirname, const char *targetFile)
{
  Serial.printf("Checking directory: %s for file: %s\r\n", dirname, targetFile);

  File root = fs.open(dirname);
  if (!root || !root.isDirectory())
  {
    Serial.println("Error opening or not a directory");
    return false;
  }

  File file = root.openNextFile();
  while (file)
  {
    if (!file.isDirectory())
    {
      if (strcmp(file.name(), targetFile) == 0)
      {
        Serial.println("File found.");
        return true;
      }
    }
    file = root.openNextFile();
  }

  Serial.println("File not found.");
  return false;
}

void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
  Serial.printf("Listing directory: %s\r\n", dirname);

  File root = fs.open(dirname);
  if (!root)
  {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    Serial.println(" - not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels)
      {
        listDir(fs, file.path(), levels - 1);
      }
    }
    else
    {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

//==============================================================

bool readBinFile(fs::FS &fs, const char *path, void *buffer, size_t bufferSize)
{
  // Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path, FILE_READ);

  if (!file || file.isDirectory())
  {
    // Serial.println("- failed to open file for reading");
    return false;
  }
  size_t bytesRead = file.readBytes((char *)buffer, bufferSize);
  file.close();

  // Serial.print("read from file:");
  Serial.write((const uint8_t *)buffer, bytesRead);
  return true;
}

bool writeBinFile(fs::FS &fs, const char *path, void *message, uint32_t len)
{
  Serial.printf("Writing binary file: %s\r\n", path);
  File file = fs.open(path, FILE_WRITE);
  if (!file)
  {
    Serial.println("- failed to open file for writing");
    return false;
  }

  size_t bytesWritten = file.write((const uint8_t *)message, len);
  if (bytesWritten == len)
  {
    Serial.println("- binary file written");
  }
  else
  {
    Serial.println("- write failed");
    return false;
  }
  file.close();
  Serial.flush();
  delay(10);
  return true;
}

bool appendBinFile(fs::FS &fs, const char *path, const char *data, uint8_t data_length)
{
  uint8_t buffer[256] = {0};
  size_t file_size = 0;
  Serial.printf("Appending to file: %s\r\n", path);
  File file = fs.open(path, FILE_READ);
  uint32_t buffet_address = 0;
  size_t bytesRead;
  if (!file || file.isDirectory())
  {
    Serial.println("- failed to open file for appending");
    file_size = 0;
  }
  else
  {
    file_size = file.size();
    bytesRead = file.readBytes((char *)buffer, file_size);
  }
  Serial.print("file_size:");
  Serial.println(file_size);
  file.close();
  memcpy(&buffer[file_size], data, data_length);

  file = fs.open(path, FILE_WRITE);

  file_size = file_size + data_length;

  size_t bytesWritten = file.write((const uint8_t *)buffer, file_size);
  if (bytesWritten == file_size)
  {
    Serial.println("- binary file written in append process");
  }
  else
  {
    Serial.println("- write failed in append process");
    return false;
  }
  file.close();
  return true;
}
