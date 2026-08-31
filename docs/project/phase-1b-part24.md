# Phase 1B Part 24

หัวข้อ: Fault Decoder Integration

รวม:

- hardware diagnostics จาก Part20
- fault contract + unit tests จาก Part21

เพิ่ม:

- human-readable ESP32 fault log
- fault name ใน `/diagnostics`
- boolean field แยก fault ทุกประเภท
- unknown fault bit reporting
- `test_fault_flags` ใน CMake test gate

Safety behavior ยังคงเดิม: `fault_flags != 0` ทำให้ hardware `read()` คืน `ERROR`

CI run 74 ยืนยันแล้วว่า ESP32 PlatformIO build และ Documentation strict build ผ่านหลัง Part23 ส่วน ROS 2 Jazzy job ยังอยู่ระหว่างดำเนินการในช่วงเริ่ม Part24
