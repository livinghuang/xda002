#include "global.h"
#include <FFat.h>

// Define partition schemes
// #define FORMAT_FFAT false

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

char *readName(fs::FS &fs)
{
  char *rst = NULL;
  static char buffer[10];
  const char *path = "/name.txt";
  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path);
  if (!file || file.isDirectory())
  {
    Serial.println("- failed to open file for reading");
    return rst;
  }
  Serial.println("- read from file:");
  while (file.available())
  {
    size_t bytesRead = file.readBytes((char *)buffer, sizeof(buffer));
    rst = buffer;
    Serial.println(rst);
  }
  file.close();
  return rst;
}

uint32_t readUpIntv(fs::FS &fs)
{
  uint32_t rst = 0;

  const char *path = "/upintv.txt";
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if (!file || file.isDirectory())
  {
    Serial.println("- failed to open file for reading");
    return rst;
  }

  Serial.println("- read from file:");
  while (file.available())
  {
    char buffer[30];
    size_t bytesRead = file.readBytes((char *)buffer, sizeof(buffer));
    rst = atol(buffer);
    Serial.println(rst);
  }
  file.close();
  return rst;
}

void readFile(fs::FS &fs, const char *path)
{
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if (!file || file.isDirectory())
  {
    Serial.println("- failed to open file for reading");
    return;
  }

  Serial.println("- read from file:");
  while (file.available())
  {
    Serial.write(file.read());
  }
  file.close();
}

void writeFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file)
  {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message))
  {
    Serial.println("- file written");
  }
  else
  {
    Serial.println("- write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Appending to file: %s\r\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file)
  {
    Serial.println("- failed to open file for appending");
    return;
  }
  if (file.print(message))
  {
    Serial.println("- message appended");
  }
  else
  {
    Serial.println("- append failed");
  }
  file.close();
}

void renameFile(fs::FS &fs, const char *path1, const char *path2)
{
  Serial.printf("Renaming file %s to %s\r\n", path1, path2);
  if (fs.rename(path1, path2))
  {
    Serial.println("- file renamed");
  }
  else
  {
    Serial.println("- rename failed");
  }
}

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

void testFileIO(fs::FS &fs, const char *path)
{
  Serial.printf("Testing file I/O with %s\r\n", path);

  static uint8_t buf[512];
  size_t len = 0;
  File file = fs.open(path, FILE_WRITE);
  if (!file)
  {
    Serial.println("- failed to open file for writing");
    return;
  }

  size_t i;
  Serial.print("- writing");
  uint32_t start = millis();
  for (i = 0; i < 2048; i++)
  {
    if ((i & 0x001F) == 0x001F)
    {
      Serial.print(".");
    }
    file.write(buf, 512);
  }
  Serial.println("");
  uint32_t end = millis() - start;
  Serial.printf(" - %u bytes written in %lu ms\r\n", 2048 * 512, end);
  file.close();

  file = fs.open(path);
  start = millis();
  end = start;
  i = 0;
  if (file && !file.isDirectory())
  {
    len = file.size();
    size_t flen = len;
    start = millis();
    Serial.print("- reading");
    while (len)
    {
      size_t toRead = len;
      if (toRead > 512)
      {
        toRead = 512;
      }
      file.read(buf, toRead);
      if ((i++ & 0x001F) == 0x001F)
      {
        Serial.print(".");
      }
      len -= toRead;
    }
    Serial.println("");
    end = millis() - start;
    Serial.printf("- %u bytes read in %lu ms\r\n", flen, end);
    file.close();
  }
  else
  {
    Serial.println("- failed to open file for reading");
  }
}
bool FORMAT_FFAT = false;
bool flash_init()
{
  bool rst = false;
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  if (FORMAT_FFAT)
    FFat.format();
  if (!FFat.begin())
  {
    Serial.println("FFat Mount Failed");
    return rst;
  }
  Serial.printf("Total space: %10u\n", FFat.totalBytes());
  Serial.printf("Free space: %10u\n", FFat.freeBytes());
  // listDir(FFat, "/", 0);
  // writeFile(FFat, "/upintv.txt", "300000");
  if (readUpIntv(FFat) == 0)
  {
    listDir(FFat, "/", 0);
    writeFile(FFat, "/upintv.txt", "3600000");
    return rst;
  }
  // writeFile(FFat, "/hello.txt", "Hello ");
  // appendFile(FFat, "/hello.txt", "World!\r\n");
  // readFile(FFat, "/hello.txt");
  // renameFile(FFat, "/hello.txt", "/foo.txt");
  // readFile(FFat, "/foo.txt");
  // deleteFile(FFat, "/foo.txt");
  // testFileIO(FFat, "/test.txt");
  // Serial.printf("Free space: %10u\n", FFat.freeBytes());
  // deleteFile(FFat, "/test.txt");
  // Serial.println("Test complete");
  // delay(1000);
  listDir(FFat, "/", 0);
  // delay(1000);
  rst = true;
  return rst;
}
