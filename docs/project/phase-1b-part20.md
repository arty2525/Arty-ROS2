# Phase 1B Part 20

หัวข้อ: Hardware-level ESP32 Diagnostics

เพิ่ม:

- `/diagnostics` publisher จาก `mobile_robot_hardware`
- ใช้ ros2_control framework-managed node
- diagnostics timer 500 ms
- atomic telemetry snapshot
- ESP32 `fault_flags`
- telemetry age / timeout state
- encoder ticks
- wheel velocity
- configured / enabled state
- inversion state
- ไม่มีการสร้าง sensor data ที่ฮาร์ดแวร์ไม่ได้วัดจริง

Part นี้ไม่เปลี่ยน UART protocol และไม่เปลี่ยน motion-control behavior
