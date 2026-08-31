# ความสมบูรณ์ของ ROS 2 Workspace สำหรับ rosdep

GitHub Actions run #90 ของ Phase1B-Part27 หยุดที่ขั้น `Install rosdep dependencies` ก่อนเข้าสู่ `colcon build`

จาก repository tree พบว่า `mobile_robot_bringup/package.xml` ประกาศ runtime dependency ชื่อ `mobile_robot_control` แต่ directory `ros2_ws/src/mobile_robot_control` ไม่มี `package.xml` และ `CMakeLists.txt`

ผลคือ ROS tooling ไม่สามารถมอง `mobile_robot_control` เป็น package ภายใน workspace ได้อย่างถูกต้อง และ dependency ชื่อนี้อาจถูกส่งต่อให้ rosdep พยายาม resolve เป็น system dependency แทน

Phase1B-Part28 จึงเพิ่ม package metadata ที่ขาด โดยไม่เพิ่มค่าฮาร์ดแวร์หรือค่าคาลิเบรตใด ๆ

## Runtime dependencies

`mobile_robot_control` ใช้ dependency ต่อไปนี้จาก launch file และ controller configuration จริง:

- `ament_index_python`
- `controller_manager`
- `diff_drive_controller`
- `joint_state_broadcaster`
- `launch`
- `launch_ros`
- `mobile_robot_description`
- `mobile_robot_hardware`
- `robot_state_publisher`
- `xacro`

package นี้เป็น `ament_cmake` package และติดตั้ง directory `launch/` กับ `config/` ไปยัง `share/mobile_robot_control`

## Verification

ต้องตรวจว่า package ถูกค้นพบโดย `colcon list` และ rosdep สามารถประมวลผล workspace ได้ก่อนเข้าสู่ build
