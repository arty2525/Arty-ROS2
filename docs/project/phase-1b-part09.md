# Phase 1B Part 09

หัวข้อ: Runtime Diagnostics Foundation

เพิ่ม `mobile_robot_diagnostics` สำหรับตรวจสอบความต่อเนื่องของ `/joint_states` และ `/diff_drive_controller/odom` พร้อมเผยแพร่ผลผ่านมาตรฐาน `diagnostic_msgs/msg/DiagnosticArray`

Part นี้ตั้งใจไม่เดาสถานะไฟฟ้า มอเตอร์ หรือ encoder ที่ยังไม่มี telemetry จริงจาก hardware interface
