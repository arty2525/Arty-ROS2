# สัปดาห์ที่ 4 — ติดตั้งและตรวจสอบ ROS 2 Jazzy

**เวลา:** 100 นาทีต่อเนื่อง

## เป้าหมาย
ตรวจ Ubuntu/architecture, source ROS environment และทดสอบ publisher/subscriber ได้

> Baseline โครงการ: Ubuntu 24.04 LTS 64-bit + ROS 2 Jazzy. ครูควรเตรียม installation ล่วงหน้าเพื่อไม่ให้ทั้งห้องดาวน์โหลดพร้อมกัน.

## กิจกรรมการเรียนรู้ 100 นาทีต่อเนื่อง
### 1. Engage — 10 นาที
เปรียบเทียบ shell ที่ source ROS environment แล้วกับ shell ที่ยังไม่ source.

### 2. Explore — 30 นาที
```bash
uname -m
cat /etc/os-release
locale
source /opt/ros/jazzy/setup.bash
printenv ROS_DISTRO
ros2 --help
```
ตรวจว่า `ROS_DISTRO=jazzy`.

### 3. Explain — 20 นาที
อธิบาย ROS distribution, environment, workspace overlay, executable และเหตุผลของการ source setup file.

### 4. Elaborate — 25 นาที
Terminal A:
```bash
source /opt/ros/jazzy/setup.bash
ros2 run demo_nodes_cpp talker
```
Terminal B:
```bash
source /opt/ros/jazzy/setup.bash
ros2 run demo_nodes_py listener
```
ระบุ publisher/subscriber และเชื่อมโยงกับหุ่นจริง: control node → topic → controller/driver; sensor/encoder → topic → processing node.

### 5. Evaluate — 15 นาที
ส่ง system verification + Talker/Listener log และตอบ ROS_DISTRO คืออะไร, ทำไม terminal ใหม่ต้อง source และถ้า listener ไม่รับข้อมูลจะตรวจ 3 อย่างแรกอะไร.

## สำหรับครู
คู่มือติดตั้งเต็ม: `docs/education/installation-guide.md`. ใช้ SD card image/เครื่อง baseline ที่ผ่านการทดสอบก่อนเริ่มสอน.
