# ESP32 Fault Flags

Phase1B-Part21 เพิ่ม host-side fault contract ที่ฝั่ง ROS 2 ใช้ถอดรหัส `fault_flags` จาก ESP32 โดยกำหนด bit layout ให้ตรงกับ firmware ปัจจุบัน

| Bit | ค่า | ชื่อ |
|---:|---:|---|
| 0 | `1 << 0` | command timeout |
| 1 | `1 << 1` | heartbeat timeout |
| 2 | `1 << 2` | invalid configuration |
| 3 | `1 << 3` | protocol error |
| 4 | `1 << 4` | invalid velocity command |

ไฟล์ `fault_flags.hpp` มีฟังก์ชัน `mask()`, `is_set()`, `known_mask()`, `unknown_bits()` และ `active_faults_text()`

หาก firmware เพิ่ม bit ใหม่ในอนาคต แต่ host ยังไม่ได้อัปเดต ระบบสามารถตรวจพบเป็น `unknown fault bits` แทนการละทิ้งข้อมูล

Part นี้สร้างและทดสอบ fault contract ก่อนนำไปผูกกับ `/diagnostics` ใน Part ถัดไป เพื่อป้องกันการเปลี่ยน diagnostic code พร้อมกับเปลี่ยน bit mapping ในครั้งเดียว
