#include "GLOBAL.h"
#include "Arduino.h"
#include "BLE.h"
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
const char *txValue;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

class MyServerCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
        pServer->getAdvertising()->stop();
    };

    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
    }
};
String convert_byte_array_to_string(uint8_t *byte_array, size_t length)
{
    String rst;
    for (size_t i = 0; i < length; i++)
    {
        // Convert the byte to a two-character hexadecimal string
        String hexValue = String(byte_array[i], HEX);

        // Ensure that the string has two characters (e.g., '0A' instead of 'A')
        if (hexValue.length() == 1)
        {
            hexValue = "0" + hexValue;
        }
        rst += hexValue;
        // Serial.print(hexValue);
    }
    return rst;
}

class MyCallbacks : public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        std::string rxValue = pCharacteristic->getValue();

        if (rxValue.length() > 0)
        {
            // or AT+TEMP=25.2
            static char buffer[256];
            memset(buffer, 0, 256);
            strncpy(buffer, rxValue.c_str(), rxValue.length());
            parseAndExecuteCommand(buffer);
        }
    }
};
char BLE_NAME[30] = "BLE-8888";
void ble_name(const char *name)
{
    strcpy(BLE_NAME, name);
}
void ble_stop(void)
{
    // Disconnect connected devices
    // pServer->disconnect(pServer->getConnId());
    // Stop advertising
    pServer->getAdvertising()->stop();
    // Deinitialize the BLE stack
    BLEDevice::deinit(BLE_NAME);
}
void ble_init()
{
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
}

bool ble_process_flag = false;
bool ble_send_flag = false;
void send_to_ble(const char *data)
{
    txValue = data;
    ble_send_flag = true;
}

void ble_process(void)
{
    if (deviceConnected)
    {
        if (ble_send_flag)
        {
            ble_send_flag = 0;
            pTxCharacteristic->setValue((uint8_t *)txValue, strlen(txValue));
            pTxCharacteristic->notify();
            delay(10); // bluetooth stack will go into congestion, if too many packets are sent
        }
    }
    else
    {
    }

    // disconnecting
    if (!deviceConnected && oldDeviceConnected)
    {
        delay(500);                  // give the bluetooth stack the chance to get things ready
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
