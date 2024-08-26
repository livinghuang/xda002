#include "global.h"
/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 6E400001-B5A3-F393-E0A9-E50E24DCCA9E
   Has a characteristic of: 6E400002-B5A3-F393-E0A9-E50E24DCCA9E - used for receiving data with "WRITE"
   Has a characteristic of: 6E400003-B5A3-F393-E0A9-E50E24DCCA9E - used to send data with  "NOTIFY"

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   In this example rxValue is the data received (only accessible inside that function).
   And txValue is the data to be sent, in this example just a byte incremented every second.
*/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer *pServer = NULL;
BLECharacteristic *pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
char txValueBuffer[255];
char BLE_NAME[20];
bool ble_processing_flag = false;
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

void generate_ble_name(void)
{
  uint64_t chipid = ESP.getEfuseMac();
  // 生成BLE名称
  snprintf(BLE_NAME, sizeof(BLE_NAME), "XDA-%04X%08X\n", (uint16_t)(chipid >> 32), (uint32_t)chipid);
  Serial.println(BLE_NAME);
}

void ble_name(const char *name)
{
  strcpy(BLE_NAME, name);
}
void ble_stop(void)
{
  // Disconnect connected devices
  pServer->disconnect(pServer->getConnId());
  // Stop advertising
  pServer->getAdvertising()->stop();
  // Deinitialize the BLE stack
  BLEDevice::deinit(BLE_NAME);
  ble_processing_flag = false;
}
class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
  }
};

class MyCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    String rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0)
    {
      Serial.println("*********");
      Serial.print("Received Value: ");
      for (int i = 0; i < rxValue.length(); i++)
        Serial.print(rxValue[i]);

      Serial.println();

      memset(ble_buffer, 0, 256);
      strncpy(ble_buffer, rxValue.c_str(), rxValue.length());
      Serial.println(ble_buffer);
      parseAndExecuteCommand(ble_buffer);

      Serial.println("*********");
    }
  }
};
unsigned long startTime;
static unsigned long waiting_connect_timeout = 30000; // Timeout in milliseconds, e.g., 5000ms for 5 seconds

void ble_init(unsigned long timeout)
{
  Serial.begin(115200);

  generate_ble_name();

  // Create the BLE Device
  BLEDevice::init(BLE_NAME);

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID_TX,
      BLECharacteristic::PROPERTY_NOTIFY);

  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID_RX,
      BLECharacteristic::PROPERTY_WRITE);

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
  startTime = millis();
  waiting_connect_timeout = timeout;
  vTaskDelay(1);
}
bool ble_send_flag = false;
void ble_send_data_ready(void)
{
  ble_send_flag = true;
}

void ble_process()
{
  if (millis() - startTime >= waiting_connect_timeout)
  {
    Serial.println("Timeout occurred. No data received within the timeout period.");
    ble_stop();
  }
  if (deviceConnected)
  {
    startTime = millis();
    if (ble_send_flag)
    {
      ble_send_flag = false;
      pTxCharacteristic->setValue((uint8_t *)txValueBuffer, strlen(txValueBuffer));
      pTxCharacteristic->notify();
      vTaskDelay(10); // bluetooth stack will go into congestion, if too many packets are sent
    }
  }
  // disconnecting
  if (!deviceConnected && oldDeviceConnected)
  {
    vTaskDelay(500);             // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected)
  {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
  }
}
