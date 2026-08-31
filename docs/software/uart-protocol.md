# UART Binary Protocol

โปรโตคอลนี้ใช้สื่อสารระหว่าง Raspberry Pi 4 และ ESP32 ผ่าน UART แบบ 8N1 ที่ 115200 baud

## การต่อสาย

| Raspberry Pi 4 | ESP32 |
|---|---|
| GPIO14 / TXD0, Physical Pin 8 | GPIO16 / RX2 |
| GPIO15 / RXD0, Physical Pin 10 | GPIO17 / TX2 |
| GND, Physical Pin 6 | GND |

TX ต้องต่อไขว้เข้ากับ RX และต้องใช้กราวด์ร่วมกัน ห้ามจ่าย 5V เข้าขา GPIO ของ ESP32

## Frame format

ข้อมูลทุกค่าเป็น little-endian

| Field | ขนาด |
|---|---:|
| Magic `0xA55A` | 2 bytes |
| Protocol version | 1 byte |
| Message type | 1 byte |
| Sequence | 2 bytes |
| Payload length | 2 bytes |
| Payload | 0–64 bytes |
| CRC-16/CCITT-FALSE | 2 bytes |

CRC ใช้ polynomial `0x1021`, initial value `0xFFFF`, no reflection และ no final XOR
