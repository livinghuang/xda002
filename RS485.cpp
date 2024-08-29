#include "global.h"

#define BAUD_RATE 9600
#define SERIAL_CONFIG SERIAL_8N1

uint16_t convetHexToDec(byte hexByte)
{
  return ((hexByte / 16) * 10 + (hexByte % 16));
}

double accumulatedValueConvert(byte *byteArray)
{
  double total_number = 0;
  for (int i = 0; i < 5; i++)
  {
    // printf("%02X ", byteArray[i + 4]);
    total_number = total_number * 100;
    uint16_t dec = convetHexToDec(*(byteArray + i));
    printf("%d ", dec);
    total_number += dec;
  }
  printf("\n");
  printf("%f\n", total_number);
  int point_index = (int)*(byteArray + 6);
  printf("point_index: %d\n", point_index);

  for (int i = 0; i < point_index; i++)
  {
    total_number = total_number / 10;
  }
  printf("\n");

  printf("%f\n", total_number);
  return total_number;
}

int16_t daysValueConvert(byte *byteArray)
{
  int16_t total_number = 0;
  for (int i = 0; i < 2; i++)
  {
    // printf("%02X ", byteArray[i + 4]);
    total_number = total_number * 100;
    uint16_t dec = convetHexToDec(*(byteArray + i));
    printf("%d ", dec);
    total_number += dec;
  }
  printf("\n");
  printf("%d\n", total_number);
  return total_number;
}
/*
L DAY：漏水天數
N DAY：負載天數
O DAY：靜止天數
U DAY：反向天數
H DAY：受磁天數
B DAY：電力不足天數
parse data
01
03
28
00
00 正向積算值 積算值 10、9
00 積算值 8、7
31 積算值 6、5
96 積算值 4、3
67 積算值 2、1
00
05 小數點位數
00
00 正向積算值 積算值 10、9
00 積算值 8、7
00 積算值 6、5
00 積算值 4、3
64 積算值 2、1
00
05 小數點位數
00 L DAY_4、3
00 L DAY_2、1
00 N DAY_4、3
00 N DAY_2、1
04 O DAY_4、3
94 O DAY_2、1
00 U DAY_4、3
00 U DAY_2、1
00 H DAY_4、3
00 H DAY_2、1
00 B DAY_4、3
00 B DAY_2、1
01 (BCD) C_4、3
35 C_2、1
0C F1
03 F2
00 過載次數
00 過載次數
52 表號 12、11
35 表號 10、9
10 表號 8、7
20 表號 6、5
11 表號 4、3
47 表號 2、1
69 77 CRC
*/

// void parse_data(byte *byteArray, int bytesRead)
// {
//   // uint8_t byteArray[] = {0x01, 0x03, 0x28, 0x00, 0x00, 0x00, 0x31, 0x96, 0x67, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x04, 0x94, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x35, 0x0C, 0x03, 0x00, 0x00, 0x52, 0x35, 0x10, 0x20, 0x11, 0x47, 0x69, 0x77};

//   float positiveAccumulatedValue = accumulatedValueConvert(&byteArray[4]);
//   Serial.printf("positiveAccumulatedValue: %f\n", positiveAccumulatedValue);
//   float negativeAccumulatedValue = accumulatedValueConvert(&byteArray[11]);
//   Serial.printf("negativeAccumulatedValue: %f\n", negativeAccumulatedValue);

//   uint16_t L_Days = daysValueConvert(&byteArray[19]);
//   Serial.printf("L_Days: %d\n", L_Days);
//   uint16_t N_Days = daysValueConvert(&byteArray[21]);
//   Serial.printf("N_Days: %d\n", N_Days);
//   uint16_t O_Days = daysValueConvert(&byteArray[23]);
//   Serial.printf("O_Days: %d\n", O_Days);
//   uint16_t U_Days = daysValueConvert(&byteArray[25]);
//   Serial.printf("U_Days: %d\n", U_Days);
//   uint16_t H_Days = daysValueConvert(&byteArray[27]);
//   Serial.printf("H_Days: %d\n", H_Days);
//   uint16_t B_Days = daysValueConvert(&byteArray[29]);
//   Serial.printf("B_Days: %d\n", B_Days);

//   memcpy(sensor_data.water_meter_data.deviceName, &byteArray[37], 6);
//   printHex(sensor_data.water_meter_data.deviceName, 6);
//   // sensor_data.water_meter_data.totalAccumulationValue = positiveAccumulatedValue - negativeAccumulatedValue;
//   // sensor_data.water_meter_data.powerInsufficiencyDays = B_Days;
//   Serial.println();
// }

void rs485_init()
{
  Serial.println("RS485 init");
  pinMode(pVext, OUTPUT);
  digitalWrite(pVext, HIGH);
  Serial1.begin(BAUD_RATE, SERIAL_CONFIG, RO, DI);
  pinMode(RE_DE, OUTPUT);
  digitalWrite(RE_DE, HIGH);
  delay(100);
}

bool checkLength(byte *responseBuffer, int bytesRead)
{
  uint16_t length = responseBuffer[2];
  Serial.printf("length: %d\n", length);
  if (length == bytesRead - 4)
  {
    return true;
  }
  return false;
}

bool checkCRC(byte *responseBuffer, int bytesRead)
{
  // uint16_t crc = crc16(responseBuffer, bytesRead - 2);
  // uint16_t receivedCRC = (responseBuffer[bytesRead - 2] << 8) | responseBuffer[bytesRead - 1];
  // if (crc == receivedCRC)
  // {
  return true;
  // }
  // return false;
}

bool checkLegalResponse(byte *responseBuffer, int bytesRead)
{
  uint8_t address = responseBuffer[0];
  uint8_t functionCode = responseBuffer[1];
  uint8_t length = responseBuffer[2];
  Serial.printf("address: 0x%02x, functionCode: 0x%02x\n", address, functionCode);

  if (address == 0x01)
  {
    Serial.println("address == 0x01");
    if (functionCode == 0x03)
    {
      Serial.println("functionCode == 0x03");
      if (length == bytesRead - 5)
      {
        Serial.print("length:");
        Serial.println(bytesRead - 5);
        if (checkCRC(responseBuffer, bytesRead))
        {
          return true;
        }
        else
        {
          Serial.println("crc error");
        }
      }
      else
      {
        Serial.printf("bytesRead: %d, length: %d ,expect length: %d\n", bytesRead, length, bytesRead - 5);
        Serial.println("length error");
      }
    }
    else
    {
      Serial.println("functionCode != 0x03");
      if (functionCode == 0x83)
      {
        Serial.println("functionCode == 0x83, error command");
      }
    }
  }
  else
  {
    Serial.println("address != 0x01");
  }
  return false;
}

void fetchDataForPowerInsufficiencyDaysAndDeviceName(byte *byteArray, int bytesRead)
{
  Serial.println("fetchDataForPowerInsufficiencyDaysAndDeviceName");
  memcpy(sensor_data.water_meter_data.deviceName, &byteArray[37], 6);
  Serial.print("Device Name:");
  printHex(sensor_data.water_meter_data.deviceName, 6);
  memcpy(sensor_data.water_meter_data.powerInsufficiencyDays, &byteArray[29], 2);
  Serial.print("Power Insufficiency Days:");
  printHex(sensor_data.water_meter_data.powerInsufficiencyDays, 2);
}

void fetchDataForTotalAccumulatedValue(byte *responseBuffer, int bytesRead)
{
  Serial.println("fetchDataForTotalAccumulatedValue");
  memcpy(sensor_data.water_meter_data.totalAccumulationValue, &responseBuffer[3], sizeof(sensor_data.water_meter_data.totalAccumulationValue));
  printHex(sensor_data.water_meter_data.totalAccumulationValue, sizeof(sensor_data.water_meter_data.totalAccumulationValue));
}

bool getDataFromRS485(byte modbusQuery[], uint8_t length, void (*parseData)(byte *byteArray, int bytesRead))
{
  Serial.print("Sent:");
  printHex(modbusQuery, length);
  uint8_t try_count = 0;
  while (try_count < 3)
  {
    digitalWrite(RE_DE, HIGH); // Enable transmission
    delay(1);
    Serial1.write(modbusQuery, length);
    Serial1.flush();
    digitalWrite(RE_DE, LOW); // Disable transmission

    byte responseBuffer[256]; // Adjust the buffer size as needed
    int bytesRead = Serial1.readBytes(responseBuffer, sizeof(responseBuffer));
    uint32_t startTime = millis();
    // Wait for the response
    while (Serial1.available() == 0 && (millis() - startTime) < 500)
    {
    }
    // Check if there is any response
    if (bytesRead > 0)
    {
      // Process the received bytes here
      Serial.print("Received:");
      printHex(responseBuffer, bytesRead);
      if (checkLegalResponse(responseBuffer, bytesRead))
      {
        parseData(responseBuffer, bytesRead);
        return true;
      }
      else
      {
        Serial.println("Illegal response");
      }
    }
    Serial.println("fetch data failed, try again");
    try_count++;
  }
  return false;
}

bool get_total_accumulated_value(void)
{
  byte modbusQuery[] = {0x01, 0x03, 0x03, 0x04, 0x00, 0x09, 0xc4, 0x49};
  return getDataFromRS485(modbusQuery, sizeof(modbusQuery), fetchDataForTotalAccumulatedValue);
}

bool get_power_insufficiency_days_and_device_name(void)
{
  byte modbusQuery[] = {0x01, 0x03, 0x05, 0x04, 0x00, 0x14, 0x04, 0xC8}; // Example Modbus query
  return getDataFromRS485(modbusQuery, sizeof(modbusQuery), fetchDataForPowerInsufficiencyDaysAndDeviceName);
}

bool rs485_process(void)
{
  Serial.println("RS485 process");
  if (get_total_accumulated_value())
  {
    Serial.println("Get total accumulated value success");
    if (get_power_insufficiency_days_and_device_name())
    {
      Serial.println("Get power insufficiency days and device name success");
      Serial.print("water_meter_data:");
      printHex((uint8_t *)&sensor_data.water_meter_data, sizeof(sensor_data.water_meter_data));
      return true;
    }
  }
  Serial.println("get data failed");
  return false;
}

void rs485_test_process()
{
  Serial.println("RS485 test process");

  digitalWrite(RE_DE, HIGH); // Enable transmission
  while (1)
  {
    Serial1.print("fetchData");
    Serial1.flush();
    delay(1000);
  }
}

bool fetch_data_process(void)
{
  get_battery_level();
  rs485_init();
  if (!rs485_process())
  {
    return false;
  }
  Serial.print("sensor_data:");
  printHex((uint8_t *)&sensor_data, sizeof(sensor_data));
  return true;
}