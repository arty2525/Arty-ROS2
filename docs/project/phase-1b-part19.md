# Phase 1B Part 19

หัวข้อ: Runtime Diagnostics Bringup Integration

แก้ไข:

- เพิ่ม dependency ที่ขาดใน `mobile_robot_diagnostics`
  - `ament_index_python`
  - `launch`
  - `launch_ros`
- ปรับ runtime monitor ให้พร้อมใช้งานกับ ROS 2 Jazzy
- integrate `diagnostics.launch.py` เข้า main `robot.launch.py`
- ตรวจ freshness ของ `/joint_states`
- ตรวจ freshness ของ `/diff_drive_controller/odom`
- ใช้ `/diagnostics` ตามมาตรฐาน ROS 2
- ไม่สร้าง telemetry ที่ฮาร์ดแวร์ไม่ได้วัดจริง
