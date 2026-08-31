# Runtime Diagnostics

Phase1B-Part09 เพิ่ม runtime diagnostics สำหรับตรวจสอบว่า ROS 2 control pipeline ยังเผยแพร่ข้อมูลตามเวลาที่กำหนดหรือไม่

## Topic ที่ตรวจสอบ

- `/joint_states`
- `/diff_drive_controller/odom`

ผลตรวจสอบเผยแพร่ผ่าน `/diagnostics` ชนิด `diagnostic_msgs/msg/DiagnosticArray`

## หลักการ

ระบบแยก communication health ออกจาก hardware health การได้รับ `/joint_states` และ `/odom` ต่อเนื่องยืนยันได้เพียงว่า ROS 2 pipeline ยังมีข้อมูลไหลผ่าน ไม่ได้ยืนยันแรงดันไฟ กระแสมอเตอร์ encoder wiring emergency stop หรือ fault ของ motor driver ดังนั้น diagnostics จะไม่สร้างสถานะฮาร์ดแวร์ที่ไม่มีข้อมูลจริงรองรับ
