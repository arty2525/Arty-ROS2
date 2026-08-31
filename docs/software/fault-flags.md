# ESP32 Fault Flags

Firmware ใช้ `uint32_t fault_flags` เป็น bitmask เพื่อส่งเหตุผิดปกติจาก ESP32 กลับมายัง ROS 2

| Bit | Hex | ชื่อ | ความหมาย |
|---:|---:|---|---|
| 0 | `0x00000001` | Command Timeout | ขณะ enable ไม่มี velocity command ใหม่ภายในเวลาที่กำหนด |
| 1 | `0x00000002` | Heartbeat Timeout | ไม่ได้รับ heartbeat จาก host ภายในเวลาที่กำหนด |
| 2 | `0x00000004` | Invalid Configuration | ได้ค่าตั้งค่าที่ไม่ valid |
| 3 | `0x00000008` | Protocol Error | payload/type ของ protocol ไม่ถูกต้อง |
| 4 | `0x00000010` | Invalid Velocity Command | velocity command เป็น NaN หรือ infinity |

Fault ที่เกี่ยวข้องกับการเคลื่อนที่จะสั่ง `stop_motion()` ทันที ซึ่งจะปิด enable, ตั้ง target เป็นศูนย์, reset PID และ PWM เป็นศูนย์

`CommandClearFaults (0x03)` ใช้ล้าง bitmask ได้ แต่จะไม่ enable มอเตอร์กลับอัตโนมัติ
