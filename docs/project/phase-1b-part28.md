# Phase1B-Part28 — ROS 2 Workspace Package Integrity

สถานะก่อนเริ่ม Part นี้:

- ESP32 PlatformIO job ผ่าน
- Documentation strict build ผ่าน
- ROS 2 Jazzy job ของ CI run #90 ล้มเหลวที่ `Install rosdep dependencies`
- build และ test ยังไม่ได้เริ่ม

ตรวจ repository tree พบ deterministic workspace defect คือ `mobile_robot_control` มีไฟล์ launch/config แต่ไม่มี `package.xml` และ `CMakeLists.txt` ขณะที่ `mobile_robot_bringup` ประกาศ dependency ไปยัง package นี้

Part นี้เพิ่ม:

- `mobile_robot_control/CMakeLists.txt`
- `mobile_robot_control/package.xml`
- dependency declarations ตาม imports และ ROS nodes ที่ launch file ใช้งานจริง
- verification สำหรับ package discovery และ rosdep

ไม่มีการกำหนด TPR, wheel separation, chassis geometry, caster geometry, PID หรือ inversion ที่ยังไม่ผ่านการยืนยัน
