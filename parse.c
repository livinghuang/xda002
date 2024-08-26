#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint16_t convetHexToDec(unsigned char hexByte)
{
  return ((hexByte / 16) * 10 + (hexByte % 16));
}

float parseByteArrayToFloat(unsigned char *byteArray)
{
  // Extract the relevant bytes
  unsigned char digits[] = {byteArray[4], byteArray[5], byteArray[6], byteArray[7], byteArray[8]};
  unsigned char decimalPointPos = byteArray[9];

  // Create a string to hold the digits
  char numStr[12]; // Ensure it's large enough to hold digits and a decimal point
  int index = 0;

  // Convert bytes to digits and add to the string
  for (int i = 0; i < 5; i++)
  {
    if (digits[i] != 0x00)
    {
      index += sprintf(&numStr[index], "%02X", digits[i]);
    }
  }

  // Insert the decimal point at the correct position
  int len = strlen(numStr);
  if (decimalPointPos <= len)
  {
    for (int i = len; i > len - decimalPointPos; i--)
    {
      numStr[i] = numStr[i - 1];
    }
    numStr[len - decimalPointPos] = '.';
    numStr[len + 1] = '\0';
  }

  // Convert the string to a float
  float result = atof(numStr);
  return result;
}

int main()
{
  // Sample byte array
  unsigned char byteArray[] = {0x01, 0x03, 0x12, 0x00, 0x00, 0x00, 0x31, 0x96, 0x00, 0x00, 0x05, 0x00, 0x00, 0x94, 0x00, 0x00, 0x00, 0x01, 0x34, 0x0C, 0x03};
  char *byteHex = &byteArray[4];
  // float result = parseByteArrayToFloat(byteArray);
  // printf("Parsed float: %f\n", result);
  float total_number = 0;
  for (int i = 0; i < 5; i++)
  {
    // printf("%02X ", byteArray[i + 4]);
    total_number = total_number * 100;
    uint16_t dec = convetHexToDec(*(byteHex + i));
    printf("%d ", dec);
    total_number += dec;
  }
  printf("\n");
  printf("%f\n", total_number);
  int point_index = (int)*(byteHex + 6);
  printf("point_index: %d\n", point_index);

  for (int i = 0; i < point_index; i++)
  {
    total_number = total_number / 10;
  }
  printf("\n");

  printf("%f", total_number);
  return 0;
}