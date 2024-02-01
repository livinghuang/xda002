#include "global.h"

#define rs485_device_address 0x01

void rs485_init()
{
  pinMode(pVext, OUTPUT);
  digitalWrite(pVext, LOW);
  pinMode(pSDN, OUTPUT);
  digitalWrite(pSDN, LOW);

  pinMode(RADIO_RESET, OUTPUT);
  digitalWrite(RADIO_RESET, LOW);
  digitalWrite(pSDN, HIGH);

  pinMode(RO485_RO, INPUT);  // 将RO引脚配置为输入
  pinMode(RO485_DI, OUTPUT); // 将DI引脚配置为输出
  pinMode(RO485_DE, OUTPUT);
  digitalWrite(RO485_DE, LOW); // Disable transmission initially
  Serial1.begin(9600, SERIAL_8N1, RO485_RO, RO485_DI);

  bool extern_24v = true;
  if (extern_24v == true)
  {
    digitalWrite(pVext, HIGH);
    digitalWrite(pSDN, HIGH);
  }
  delay(5000);
}

byte responseBuffer[256];
void rs485_process(void)
{
  byte modbusQuery[] = {rs485_device_address, 0x03, 0x00, 0x00, 0x00, 0x08, 0x44, 0x0C}; // Example Modbus query
  uint8_t fetch_times = 3;
  while (fetch_times--)
  {
    Serial.print("Sent:");
    printHex(modbusQuery, sizeof(modbusQuery));
    digitalWrite(RO485_DE, HIGH); // Enable transmission
    Serial1.write(modbusQuery, sizeof(modbusQuery));
    Serial1.flush();
    digitalWrite(RO485_DE, LOW); // Disable transmission
    while (1)
    {
      // byte responseBuffer[256]; // Adjust the buffer size as needed
      int bytesRead = Serial1.readBytes(responseBuffer, sizeof(responseBuffer));

      if (bytesRead > 0)
      {
        // Process the received bytes here
        Serial.print("Received:");
        if (responseBuffer[0] == rs485_device_address)
        {
          printHex(responseBuffer, bytesRead);
        }
        else
        {
          Serial.println("No response received.");
        }
        break;
      }
      delay(50);
    }
    Serial.flush();
    delay(200);
  }
}