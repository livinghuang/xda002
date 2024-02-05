# User Guide for AT Commands

This guide provides an explanation of various AT commands that you can use to interact with your device.

## AT

- **Function**: Check if the AT command is available and functional.

## AT+NAME

- **Function**: Get the device name.
- **Usage**:
  - `AT+NAME`: Retrieve the device name.

## AT+BOARD

- **Function**: Get the board information.
- **Usage**:
  - `AT+BOARD`: Retrieve information about the device's board.

## AT+CALIB

- **Function**: Get calibration information or start calibration.
- **Usage**:
  - `AT+CALIB=0` or `AT+CALIB=?`: Get calibration information.
  - `AT+CALIB=1`: Start the calibration process.

## AT+TEMPOFFSET

- **Function**: Get temperature offset information.

## AT+UPINTV

- **Function**: Get or set the uplink interval time in microseconds.
- **Usage**:
  - `AT+UPINTV=?`: Get the current uplink interval time.
  - `AT+UPINTV=180000`: Set the uplink interval time to 180,000 microseconds.

## AT+FORMAT

- **Function**: Format storage space, which also clears board and calibration information.

## AT+CLRMEM

- **Function**: Clear storage space.

These AT commands are used to configure and retrieve information from your device. Be sure to follow the provided usage instructions for each command to achieve the desired functionality.
