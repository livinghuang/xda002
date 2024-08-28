import serial
import struct

# Replace '/dev/tty.usbserial-21440' with your serial port
serial_port = '/dev/tty.usbserial-21440'
baud_rate = 4800  # Set the baud rate according to your device

ser = serial.Serial(serial_port, baud_rate, timeout=1)

# Define the commands and their corresponding responses
commands_responses = {
    b'\x01\x03\x03\x04\x00\x09\xc4\x49': b'\x01\x03\x12\x00\x00\x00\x31\x96\x00\x00\x05\x00\x00\x94\x00\x00\x00\x01\x34\x0C\x03\x89\x60',  # Example command 1 and its response
                                            #01 03 12 00 00 00 31 96 00 00 05 00 00 94 00 00 00 01 34 0C 03 89 60
    b'\x01\x03\x05\x04\x00\x14\x04\xc8': b'\x01\x03\x28\x00\x00\x00\x31\x96\x67\x00\x05\x00\x00\x00\x00\x00\x64\x00\x05\x00\x00\x00\x00\x04\x94\x00\x00\x00\x00\x00\x00\x01\x35\x0C\x03\x00\x00\x52\x35\x10\x20\x11\x47\x69\x77',  # Example command 2 and its response
                                            #01  03  28  00  00  00  31  96  67  00  05  00  00  00  00  00  64  00  05  00  00  00  00  04  94  00  00  00  00  00  00  01  35  0C  03  00  00  52  35  10  20  11  47  69  77
    # Add more command-response pairs here
}

try:
    while True:
        line = ser.readline().strip()

        if line:
            print(f"Received (raw bytes): {line}")
            print(f"Received (hex): {line.hex()}")

            # Check if the received command matches any predefined command
            response = commands_responses.get(line, None)

            if response:
                ser.write(response)
                print(f"Sent (raw bytes): {response}")
                print(f"Sent (hex): {response.hex()}")
            else:
                print("Unknown command received.")

except KeyboardInterrupt:
    print("Exiting program")

ser.close()