// #include <stdio.h>
// #include <stdint.h>

// // Define the CRC-16/MODBUS polynomial and initial values
// #define POLYNOMIAL 0x8005
// #define INITIAL_REMAINDER 0xFFFF
// #define FINAL_XOR_VALUE 0x0000

// uint16_t crc16_modbus(const uint8_t *data, uint16_t length)
// {
//   uint16_t remainder = INITIAL_REMAINDER;

//   for (uint16_t byte = 0; byte < length; ++byte)
//   {
//     remainder ^= data[byte];

//     for (uint8_t bit = 8; bit > 0; --bit)
//     {
//       if (remainder & 0x0001)
//       {
//         remainder = (remainder >> 1) ^ POLYNOMIAL;
//       }
//       else
//       {
//         remainder = (remainder >> 1);
//       }
//     }
//   }

//   return (remainder ^ FINAL_XOR_VALUE);
// }

// int main()
// {
//   // Your hexadecimal data
//   uint8_t data[] = {
//       0x01, 0x03, 0x28, 0x00, 0x00, 0x00, 0x31, 0x96,
//       0x67, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00,
//       0x64, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x04,
//       0x94, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x35,
//       0x0C, 0x03, 0x00, 0x00, 0x52, 0x35, 0x10, 0x20,
//       0x11, 0x47};

//   uint16_t length = sizeof(data) / sizeof(data[0]);

//   // Calculate CRC
//   uint16_t crc = crc16_modbus(data, length);

//   // Print CRC
//   printf("CRC-16/MODBUS: %04X\n", crc);

//   return 0;
// }