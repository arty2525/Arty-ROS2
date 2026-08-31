# UART Frame Resynchronization

Phase1B-Part17 ปรับ parser ฝั่ง ESP32 ให้ค้นหา protocol magic แบบ byte-by-byte แทนการสมมติว่า UART stream เริ่มตรงกับต้น frame เสมอ

Protocol magic คือ `0xA55A`

บน ESP32 ซึ่งใช้ little-endian จะรับ magic bytes เป็น:

- `0x5A`
- `0xA5`

Parser จะ:

1. รอ `0x5A`
2. ตรวจ byte ถัดไปเป็น `0xA5`
3. อ่านส่วนที่เหลือของ Header
4. ตรวจ protocol version
5. ตรวจ payload size
6. อ่าน payload
7. ตรวจ CRC16 CCITT-FALSE
8. หาก frame ไม่ถูกต้อง จะกลับไปค้นหา magic ใหม่โดยไม่ล้าง UART receive buffer ทั้งหมด

ผลคือ noise, truncated frame หรือ CRC error หนึ่ง frame ไม่ควรทำให้ frame ที่ถูกต้องหลังจากนั้นสูญหายต่อเนื่อง

## Streaming semantics

`CommandVelocity` และ `Heartbeat` ไม่มี ACK

คำสั่งที่ต้องมี ACK:

- Configure
- Enable
- Set PID
- Clear Faults

## Heartbeat watchdog

Heartbeat timeout ทำงานเฉพาะขณะ firmware อยู่ในสถานะ `enabled`

ดังนั้น ESP32 ที่เปิดเครื่องแต่ยังไม่ได้ configure/activate จะไม่สร้าง `HeartbeatTimeout` fault เอง

เมื่อได้รับ Enable=true:

- reset command timer
- reset heartbeat timer
- clear timeout fault เดิม

เมื่อ heartbeat หายเกินเวลาที่กำหนดขณะ enabled:

- set `HeartbeatTimeout`
- stop PWM
- disable motor control
