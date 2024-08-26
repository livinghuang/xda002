# XDA002 - LoRaWAN Liquid Flow Meter

Welcome to the XDA002 project! XDA002 is designed to fetch readings from an RMT water meter and forward them to a LoRaWAN system using ABP mode.

## Project Overview

The XDA002 reads data from the RMT water meter via RS485.

## Liquid Flow Meter Integration

This integration involves connecting the liquid flow meter to the Heltec HT-CT62 module via the MAX485 interface. This setup ensures precise collection of flow data, which is then prepared for upload to the LoRaWAN network.

## LoRaWAN Data Upload

Collected data from the liquid flow meter is transmitted to the LoRaWAN network, providing wide-area connectivity for remote monitoring and analysis of liquid flow metrics.

## Byte Array Structure

The following table explains the byte array structure for the `sensor_data_t` and `WaterMeterData_t` structs:

| **Field**                        | **Data Type**  | **Size (Bytes)** | **Description**                | **Index**        |
|----------------------------------|----------------|------------------|--------------------------------|------------------|
| `main_battery_level`             | `uint8_t`      | 1                | Main battery level             | 0                |
| `reserve`                        | `uint8_t[3]`   | 3                | Reserved bytes                 | 1 - 3            |
| `totalAccumulationValue`         | `byte[8]`      | 8                | Total Accumulation Value       | 4 - 11           |
| `powerInsufficiencyDays`         | `uint8_t[2]`   | 2                | Power Insufficiency Days       | 12 - 13          |
| `deviceName`                     | `byte[6]`      | 6                | Device Name                    | 14 - 19          |

The overall `sensor_data_t` structure is 20 bytes long. Here’s how the byte array corresponds to each field within the structs:

### `sensor_data_t` Structure:

- **Index 0**: `main_battery_level` (1 byte)
- **Index 1-3**: `reserve` (3 bytes)

### `WaterMeterData_t` Embedded in `sensor_data_t`:

- **Index 4-11**: `totalAccumulationValue` (8 bytes)
- **Index 12-13**: `powerInsufficiencyDays` (2 bytes)
- **Index 14-19**: `deviceName` (6 bytes)

### Example Byte Array

Given byte array: `6e ff ff ff 00 00 00 31 96 02 00 05 00 00 00 52 35 10 20 11 47`

### Byte Array Breakdown

| **Index** | **Value** |
|-----------|-----------|
| 0         | 6e        |
| 1         | ff        |
| 2         | ff        |
| 3         | ff        |
| 4         | 00        |
| 5         | 00        |
| 6         | 00        |
| 7         | 31        |
| 8         | 96        |
| 9         | 02        |
| 10        | 00        |
| 11        | 05        |
| 12        | 00        |
| 13        | 00        |
| 14        | 52        |
| 15        | 35        |
| 16        | 10        |
| 17        | 20        |
| 18        | 11        |
| 19        | 47        |

### Parsing the Byte Array

1. **`main_battery_level`**:
   - **Index 0**: `6e` (hex) = 110 (decimal)
   - **Value**: 110

2. **`reserve`**:
   - **Index 1-3**: `ff ff ff` (hex) = `255 255 255` (decimal)
   - **Value**: 255, 255, 255

3. **`totalAccumulationValue`**:
   - **Index 4-11**: `00 00 00 31 96 02 00 05` (hex)
   - **Value**: `00 00 00 31 96 02 00 05`

4. **`powerInsufficiencyDays`**:
   - **Index 12-13**: `00 00` (hex) = 0 (decimal)
   - **Value**: 0

5. **`deviceName`**:
   - **Index 14-19**: `52 35 10 20 11 47` (hex)
   - **Value**: `52 35 10 20 11 47`

### Parsed Values

```plaintext
main_battery_level: 110
reserve: [255, 255, 255]
totalAccumulationValue: [00, 00, 00, 31, 96, 02, 00, 05]
powerInsufficiencyDays: 0
deviceName: [52, 35, 10, 20, 11, 47]
```

## Getting Started

### Power Connection

Connect the power supply to the power socket (XH2.54-2) for power input. Another port (XH2.54-4) is used for the RMT, internally connecting the power path to power on the RMT and communicate with it.

**Power Input Range**: 6~7V

### Retrieving Board Information

Use a USB connection to the XDA002 and access the serial COM port to read the log. When the system starts, it will display the device board information:

```plaintext
Hello XDA002
ESP32ChipID=B0DF9B27843C
devEUI:3C84279BDFB00000
devAddr:88350FEA
nwkSKey:30303030623064663962323738343363
appSKey:30303030623064663962323738343363
BLE-1295323
```

### Setting Parameters

XDA002 has a default upload interval time of 1 hour (360000 ms). Users can set the interval time via BLE. When the system powers on, there is a 10-second window for BLE settings. During this period, connect to the XDA002 and use the following commands.

**Command 1**: `AT`
- **Description**: Test command
- **Parameter**: None
- **Response**: `AT OK`

**Example**:
- **Send**: `AT`
- **Response**: `AT OK`

**Command 2**: `AT+UPINTV`
- **Description**: Set the upload interval time (max interval time: 604,800,000 ms or 7 days)
- **Parameter**: `?` or `"interval time in ms"`
- **Response**: `"interval time", OK`

**Example 1**: (get current interval time)
- **Send**: `AT+UPINTV=?`
- **Response**: `60000 ,OK`

**Example 2**: (set interval time to 1 hour)
- **Send**: `AT+UPINTV=360000`
- **Response**: `360000 ,OK`

## Contributions and Feedback

We welcome any feedback. If you have suggestions or improvements, please feel free to open an issue or submit a pull request.

# XDA002 - LoRaWAN 液體流量計

歡迎來到 XDA002 專案！XDA002 設計用於從 RMT 水表讀取數據並使用 ABP 模式將其轉發至 LoRaWAN 系統。

## 專案概述

XDA002 通過 RS485 從 RMT 水表讀取數據。

## 液體流量計集成

集成過程包括通過 MAX485 介面將液體流量計連接到 Heltec HT-CT62 模組。此設置可確保精確收集流量數據，然後準備上傳至 LoRaWAN 網絡。

## LoRaWAN 數據上傳

從液體流量計收集的數據會傳輸到 LoRaWAN 網絡，提供廣域連接以進行液體流量指標的遠程監控和分析。

## 位元組陣列結構

以下表格解釋了 `sensor_data_t` 和 `WaterMeterData_t` 結構的位元組陣列結構：

| **欄位**                        | **數據類型**  | **大小（位元組）** | **描述**                | **索引**        |
|----------------------------------|----------------|------------------|--------------------------------|------------------|
| `main_battery_level`             | `uint8_t`      | 1                | 主電池電量              | 0                |
| `reserve`                        | `uint8_t[3]`   | 3                | 保留位元組                 | 1 - 3            |
| `totalAccumulationValue`         | `byte[8]`      | 8                | 總累積值                 | 4 - 11           |
| `powerInsufficiencyDays`         | `uint8_t[2]`   | 2                | 電力不足天數             | 12 - 13          |
| `deviceName`                     | `byte[6]`      | 6                | 設備名稱                | 14 - 19          |

整個 `sensor_data_t` 結構為 20 個位元組長。以下是位元組陣列與結構中每個欄位的對應方式：

### `sensor_data_t` 結構：

- **索引 0**: `main_battery_level` (1 位元組)
- **索引 1-3**: `reserve` (3 位元組)

### `WaterMeterData_t` 嵌入在 `sensor_data_t` 中：

- **索引 4-11**: `totalAccumulationValue` (8 位元組)
- **索引 12-13**: `powerInsufficiencyDays` (2 位元組)
- **索引 14-19**: `deviceName` (6 位元組)

### 範例位元組陣列

給定位元組陣列：`6e ff ff ff 00 00 00 31 96 02 00 05 00 00 00 52 35 10 20 11 47`

### 位元組陣列細分

| **索引** | **值** |
|-----------|-----------|
| 0         | 6e        |
| 1         | ff        |
| 2         | ff        |
| 3         | ff        |
| 4         | 00        |
| 5         | 00        |
| 6         | 00        |
| 7         | 31        |
| 8         | 96        |
| 9         | 02        |
| 10        | 00        |
| 11        | 05        |
| 12        | 00        |
| 13        | 00        |
| 14        | 52        |
| 15        | 35        |
| 16        | 10        |
| 17        | 20        |
| 18        | 11        |
| 19        | 47        |

### 解析位元組陣列

1. **`main_battery_level`**:
   - **索引 0**: `6e` (16 進位) = 110 (10 進位)
   - **值**: 110

2. **`reserve`**:
   - **索引 1-3**: `ff ff ff` (16 進位) = `255 255 255` (10 進位)
   - **值**: 255, 255, 255

3. **`totalAccumulationValue`**:
   - **索引 4-11**: `00 00 00 31 96 02 00 05` (16 進位)
   - **值**: `00 00 00 31 96 02 00 05`

4. **`powerInsufficiencyDays`**:
   - **索引 12-13**: `00 00` (16 進位) = 0 (10 進位)
   - **值**: 0

5. **`deviceName`**:
   - **索引 14-19**: `52 35 10 20 11 47` (16 進位)
   - **值**: `52 35 10 20 11 47`

### 解析結果

```plaintext
main_battery_level: 110
reserve: [255, 255, 255]
totalAccumulationValue: [00, 00, 00, 31, 96, 02, 00, 05]
powerInsufficiencyDays: 0
deviceName: [52, 35, 10, 20, 11, 47]
```

## 快速入門

### 電源連接

將電源供應連接到電源插座 (XH2.54-2) 進行電源輸入。另一個端口 (XH2.54-4) 用於 RMT，內部連接電源路徑以啟動 RMT 並與其通信。

**電源輸入範圍**: 6~7V

### 獲取板卡信息

使用 USB 連接 XDA002，然後使用串行 COM 端口讀取日誌。系統啟動時會顯示設備板卡信息：

```plaintext
Hello XDA002
ESP32ChipID=B0DF9B27843C
devEUI:3C84279BDFB00000
devAddr:88350FEA
nwkSKey:30303030623064663962323738343363
appSKey:30303030623064663962323738343363
BLE-1295323
```

### 設置參數

XDA002 預設的上傳間隔時間為 1 小時（360000 毫秒）。用戶可以通過 BLE 設置間隔時間。系統啟動時，有 10 秒的時間進行 BLE 設置。在此期間，可以連接到 XDA002 並使用以下命令。

**命令 1**: `AT`
- **描述**: 測試命令
- **參數**: 無
- **回應**: `AT OK`

**範例**:
- **發送**: `AT`
- **回應**: `AT OK`

**命令 2**: `AT+UPINTV`
- **描述**: 設置上傳間隔時間（最大間隔時間：604,800,000 毫秒或 7 天）
- **參數**: `?` 或 `"間隔時間（毫秒）"`
- **回應**: `"間隔時間", OK`

**範例 1**: (獲取當前間隔時間)
- **發送**: `AT+UPINTV=?`
- **回應**: `60000 ,OK`

**範例 2**: (將間隔時間設置為 1 小時)
- **發送**: `AT+UPINTV=360000`
- **回應**: `360000 ,OK`

## 貢獻與反饋

我們歡迎任何反饋。如果您有建議或改進，請隨時提出問題或提交請求。