# mobile_robot_diagnostics

แพ็กเกจตรวจสอบความสดใหม่ของข้อมูล runtime โดยไม่เดาสถานะของฮาร์ดแวร์ที่ระบบยังไม่ได้ expose ออกมา

ตรวจสอบ `/joint_states` และ `/diff_drive_controller/odom` แล้วเผยแพร่ผลที่ `/diagnostics` ด้วย `diagnostic_msgs/msg/DiagnosticArray`

ระดับสถานะคือ OK เมื่อข้อมูลสดใหม่ WARN เมื่อยังไม่เคยได้รับข้อมูล และ ERROR เมื่อข้อมูลขาดหายเกิน timeout
