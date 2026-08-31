# Phase 1B Part 15

หัวข้อ: End-to-End Hardware Validation Gate

เพิ่ม production gate ก่อนใช้งานฐานขับเคลื่อนจริง:

- exact wheel joint mapping ตามชื่อ ไม่อิงลำดับใน URDF
- exact state/command interface validation
- UART baud validation ตั้งแต่ `on_init`
- configurable telemetry watchdog
- non-finite telemetry rejection
- ESP32 `fault_flags != 0` ทำให้ hardware interface คืน ERROR
- ขั้นตอน validation และ calibration บน Raspberry Pi จริง
