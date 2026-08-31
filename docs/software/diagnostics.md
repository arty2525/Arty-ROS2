# Diagnostics

Arty-ROS2 ใช้ topic มาตรฐาน:

`/diagnostics`

Phase1B-Part19 เปิดใช้งาน runtime diagnostics พร้อม main robot bringup โดยอัตโนมัติ

## Runtime Topic Diagnostics

`mobile_robot_diagnostics` ตรวจ freshness ของ:

- `/joint_states`
- `/diff_drive_controller/odom`

ค่า default:

- publish rate 2 Hz
- joint state timeout 1 s
- odometry timeout 1 s

ระดับสถานะ:

- `OK`: topic ยังได้รับข้อมูลภายใน timeout
- `WARN`: node ยังไม่เคยได้รับข้อมูลจาก topic
- `ERROR`: topic ขาดข้อมูลนานกว่า timeout

## Bringup

คำสั่งหลัก:

```bash
ros2 launch mobile_robot_bringup robot.launch.py
```

จะเปิดทั้ง control stack และ:

`mobile_robot_diagnostics/diagnostics.launch.py`

ตรวจผลด้วย:

```bash
ros2 topic echo /diagnostics
```

แพ็กเกจนี้รายงานเฉพาะสถานะที่ตรวจสอบได้จาก ROS 2 topic และไม่สร้างค่ากระแส แรงดัน อุณหภูมิ หรือข้อมูลฮาร์ดแวร์ที่ไม่มี sensor จริง

Hardware-level ESP32 fault diagnostics จะผูกเข้ากับ framework-managed ros2_control hardware node ใน Part ถัดไป
