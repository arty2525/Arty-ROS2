# Phase 1B Part 21

หัวข้อ: ESP32 Fault Contract + Unit Tests

เพิ่ม:

- `mobile_robot_hardware/fault_flags.hpp`
- bit mapping ตรงกับ `firmware/esp32/include/faults.hpp`
- human-readable fault text
- unknown fault bit detection
- unit tests ป้องกัน host/firmware mapping คลาดเคลื่อน
- CMake test target `test_fault_flags`

Part นี้ไม่เปลี่ยน firmware, UART protocol, motor control หรือ lifecycle behavior
