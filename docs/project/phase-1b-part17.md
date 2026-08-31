# Phase 1B Part 17

หัวข้อ: Firmware UART Resynchronization + Watchdog Semantics

แก้จุดตกค้างฝั่ง ESP32:

- ยกเลิก hardcoded inversion ของล้อขวา
- ใช้ inversion จาก `ConfigurePayload` จริงทั้ง 4 ค่า
- validate inversion byte เป็น 0/1
- heartbeat watchdog ทำงานเฉพาะตอน enabled
- Enable=true reset watchdog timers
- parser ค้นหา magic แบบ byte-by-byte
- CRC/header error ไม่ทำให้ parser สมมติ frame alignment ผิดต่อเนื่อง
- streaming Velocity/Heartbeat ไม่มี ACK
