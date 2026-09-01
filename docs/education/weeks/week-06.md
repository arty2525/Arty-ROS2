# สัปดาห์ที่ 6 — ROS 2 Workspace, Package และ Colcon

**เวลา:** 100 นาทีต่อเนื่อง

## จุดประสงค์
อธิบาย workspace/package, ตรวจด้วย `colcon list`, อ่าน `package.xml`, ติดตั้ง dependency และ build workspace ได้

## กิจกรรมการเรียนรู้ 100 นาทีต่อเนื่อง
### 1. Engage — 10 นาที
เปรียบเทียบโครงการไฟล์จำนวนมากที่ไม่มีโครงสร้างกับ ROS workspace ที่แบ่ง package.

### 2. Explore — 30 นาที
```bash
cd ~/Arty-ROS2/ros2_ws
find src -maxdepth 2 -name package.xml -print
colcon list
```
ค้นหา package หลักและเปิด `package.xml` เพื่อหาชื่อ/version/maintainer/license/dependency/build type.

### 3. Explain — 20 นาที
อธิบาย `src/`, `build/`, `install/`, `log/`, dependency, ament และ overlay workspace.

### 4. Elaborate — 25 นาที
```bash
source /opt/ros/jazzy/setup.bash
cd ~/Arty-ROS2/ros2_ws
rosdep install --from-paths src --ignore-src --rosdistro jazzy -y
colcon build --symlink-install
source install/setup.bash
ros2 pkg list | grep mobile_robot
```
หากผิดพลาด ให้เก็บ error ก่อนแก้และค้น “สาเหตุแรก” ไม่ใช่อ่านเฉพาะบรรทัดสุดท้าย.

### 5. Evaluate — 15 นาที
ส่ง package map + build log + อธิบาย `source install/setup.bash`.

## Engineering Rule
ห้ามเปลี่ยนค่าฮาร์ดแวร์ที่ยังไม่ทราบเพื่อบังคับให้ launch ผ่าน; ต้องวัดจริงก่อน configure.
