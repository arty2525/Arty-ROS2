# Hardware Fault Diagnostics

Phase1B-Part24 เชื่อม fault contract ที่ทดสอบแล้วเข้ากับ hardware diagnostics ของ `mobile_robot_hardware`

เมื่อ ESP32 ส่ง `fault_flags != 0`:

- `hardware read()` ยังคงคืน `ERROR`
- ROS 2 log แสดงทั้งค่า bitmask และชื่อ fault
- `/diagnostics` แสดง fault รายตัว
- bit ที่ host ยังไม่รู้จักถูกเก็บเป็น `unknown_fault_bits`

Fields:

- `active_faults`
- `command_timeout`
- `heartbeat_timeout`
- `invalid_configuration`
- `protocol_error`
- `invalid_velocity_command`
- `unknown_fault_bits`

Bit mapping ตรงกับ `firmware/esp32/include/faults.hpp`:

- bit 0: command timeout
- bit 1: heartbeat timeout
- bit 2: invalid configuration
- bit 3: protocol error
- bit 4: invalid velocity command

หากพบ bit ที่ host ยังไม่รู้จัก ระบบจะรายงานผ่าน `unknown_fault_bits` โดยไม่ละทิ้งข้อมูล

Part นี้ไม่ clear fault อัตโนมัติขณะ active และไม่ลด safety behavior เดิม
