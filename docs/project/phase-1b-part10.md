# Phase 1B Part 10

หัวข้อ: ESP32 Hardware Telemetry Diagnostics Integration

เพิ่ม diagnostics จาก `mobile_robot_hardware` โดยตรงผ่าน framework-managed ROS 2 node ของ `ros2_control`

ค่าจาก ESP32 telemetry ถูกเก็บสำหรับ diagnostic publisher แบบ thread-safe โดยไม่ให้ publisher อ่านตัวแปร control-loop ที่เกิด data race

เพิ่ม telemetry timeout เป็น hardware parameter แบบ optional ค่าเริ่มต้น 500 ms
