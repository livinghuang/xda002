#include "global.h"

#define BAUD_RATE 9600
#define SERIAL_CONFIG SERIAL_8N1
void rs485_init()
{
  Serial.println("RS485 init");
  pinMode(pVext, OUTPUT);
  digitalWrite(pVext, LOW);

  Serial1.begin(BAUD_RATE, SERIAL_CONFIG, RO, DI);
  pinMode(RE_DE, OUTPUT);
  pinMode(pLORA_RST, OUTPUT);
  digitalWrite(RE_DE, HIGH);
  digitalWrite(pLORA_RST, LOW);
}
void rs485_process()
{
  Serial.println("RS485 process");
  Serial1.printf("hello\r\n");
}