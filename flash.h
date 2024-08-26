#ifndef FLASH_H
#define FLASH_H
#include "global.h"
extern bool FORMAT_FFAT;
bool flash_init();
bool fileExistsInDir(fs::FS &fs, const char *dirname, const char *targetFile);
void listDir(fs::FS &fs, const char *dirname, uint8_t levels);
void readFile(fs::FS &fs, const char *path);
bool readBinFile(fs::FS &fs, const char *path, void *buffer, size_t bufferSize);
bool writeBinFile(fs::FS &fs, const char *path, void *message, uint32_t len);
void writeFile(fs::FS &fs, const char *path, const char *message);
void appendFile(fs::FS &fs, const char *path, const char *message);
void renameFile(fs::FS &fs, const char *path1, const char *path2);
void deleteFile(fs::FS &fs, const char *path);
bool appendBinFile(fs::FS &fs, const char *path, const char *data, uint8_t data_length);

uint32_t readUpIntv(fs::FS &fs);
char *readName(fs::FS &fs);
#endif