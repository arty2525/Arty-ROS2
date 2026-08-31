# Hardware Diagnostics

Phase1B-Part20 เพิ่ม hardware diagnostics โดยตรงจาก `mobile_robot_hardware`

Topic:

`/diagnostics`

Diagnostic status name:

`Arty-ROS2 hardware: mobile_robot_hardware`

## ข้อมูลที่รายงาน

- serial device
- UART baud rate
- ticks per revolution
- telemetry timeout
- configured state
- enabled state
- telemetry availability
- telemetry age
- ESP32 fault flags
- left encoder ticks
- right encoder ticks
- left wheel velocity
- right wheel velocity
- motor inversion
- encoder inversion

ข้อมูลเหล่านี้มาจาก configuration และ ESP32 telemetry ที่มีอยู่จริง

ระบบจะไม่สร้างค่ากระแส แรงดัน อุณหภูมิ หรือแบตเตอรี่ หากไม่มี sensor วัดจริง

## ระดับสถานะ

### OK

เกิดเมื่อ hardware configure สำเร็จ ได้รับ telemetry ภายใน timeout และ `fault_flags == 0`

### WARN

เกิดเมื่อ hardware ยังไม่ configure หรือ configure แล้วแต่ยังไม่ได้ telemetry frame แรก

### ERROR

เกิดเมื่อ telemetry age เกิน `telemetry_timeout_ms` หรือ ESP32 ส่ง `fault_flags != 0`

## Thread safety

callback timer ของ diagnostics ไม่อ่านค่าจาก control loop โดยตรง แต่ใช้ atomic snapshot ที่อัปเดตจาก telemetry เพื่อหลีกเลี่ยง data race ระหว่าง ros2_control loop กับ executor timer

## ตรวจสอบ

```bash
ros2 topic echo /diagnostics
```

กรอง hardware diagnostic:

```bash
ros2 topic echo /diagnostics | grep -A 30 "Arty-ROS2 hardware"
```
