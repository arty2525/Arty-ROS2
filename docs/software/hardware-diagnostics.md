# Hardware Diagnostics

`mobile_robot_hardware` เผยแพร่สถานะจริงที่ได้รับจาก ESP32 ไปยัง `/diagnostics` โดยใช้ framework-managed node ของ `ros2_control`

ข้อมูลที่เผยแพร่ประกอบด้วย:

- UART device และ baud rate
- อายุของ telemetry ล่าสุด
- `configured`
- `enabled`
- `fault_flags`
- left/right encoder ticks
- left/right wheel velocity

## ระดับสถานะ

`ERROR` เมื่อ:

- firmware ไม่อยู่ในสถานะ configured
- telemetry ไม่เคยเข้าหรือขาดเกิน timeout
- `fault_flags != 0`

`OK` เมื่อ firmware configured, telemetry ยังสด และไม่มี fault flags

สถานะ disabled ไม่ถือเป็น fault เพราะเป็นสภาวะปกติเมื่อ controller ยังไม่ active หรือระบบหยุดมอเตอร์

## ขอบเขต

ข้อมูลนี้มาจาก telemetry ของ ESP32 โดยตรง แต่ `fault_flags` จะมีความหมายได้เท่าที่ firmware มี detector รองรับเท่านั้น

ดังนั้นยังไม่ตีความว่าแรงดัน, กระแส, อุณหภูมิ หรือ emergency stop ปกติ หากไม่มี sensor/telemetry สำหรับข้อมูลเหล่านั้น
