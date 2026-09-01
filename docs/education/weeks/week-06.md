# สัปดาห์ที่ 6 — ROS 2 Workspace, Package และ Colcon

**เวลา:** 100 นาที

## จุดประสงค์
นักเรียนอธิบาย workspace/package ได้, ตรวจ package ด้วย `colcon list`, เข้าใจ `package.xml` และ build workspace ได้.

## คาบที่ 1
### Engage — 10 นาที
ให้นักเรียนเปรียบเทียบโครงการที่มีไฟล์จำนวนมากโดยไม่มีโครงสร้าง กับ ROS workspace ที่แบ่งเป็น package.

### Explore — 30 นาที
```bash
cd ~/Arty-ROS2/ros2_ws
find src -maxdepth 2 -name package.xml -print
colcon list
```
ให้นักเรียนค้นหา package หลัก เช่น `mobile_robot_bringup`, `mobile_robot_control`, `mobile_robot_description`, `mobile_robot_diagnostics`, `mobile_robot_hardware`.

เปิด `package.xml` ของหนึ่ง package และหาชื่อ, version, maintainer, license, dependency และ build type.

### Explain — 10 นาที
อธิบาย `src/`, `build/`, `install/`, `log/`, dependency และ overlay workspace.

## คาบที่ 2
### Explore/Elaborate — 30 นาที
```bash
source /opt/ros/jazzy/setup.bash
cd ~/Arty-ROS2/ros2_ws
rosdep install --from-paths src --ignore-src --rosdistro jazzy -y
colcon build --symlink-install
source install/setup.bash
ros2 pkg list | grep mobile_robot
```

หาก build ไม่ผ่าน นักเรียนต้องบันทึก error ก่อนแก้ ห้ามลบข้อความผิดพลาด.

### Explain — 5 นาที
ครูสาธิตวิธีอ่าน error จาก “บรรทัดแรกที่เป็นสาเหตุ” แทนการคัดเฉพาะบรรทัดสุดท้าย.

### Evaluate — 15 นาที
ส่ง package map + build log + อธิบายว่า `source install/setup.bash` ทำหน้าที่อะไร.

## Safety/Engineering Rule
การ build software ไม่อนุญาตให้แก้ค่าฮาร์ดแวร์ที่ยังไม่ทราบเพียงเพื่อให้ launch ผ่าน. ค่าที่ไม่ทราบต้องคง fail-safe จนวัดจริง.
