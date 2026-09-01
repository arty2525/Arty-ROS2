# สัปดาห์ที่ 11 — URDF, Xacro และ TF: สร้างตัวตนของหุ่นยนต์ใน ROS 2

**รายวิชา:** ระบบปฏิบัติการหุ่นยนต์ • **ระดับชั้น:** ม.4 • **เวลา:** 100 นาทีต่อเนื่อง

## จุดประสงค์
นักเรียนสามารถอธิบาย link/joint/frame, อ่าน Xacro ของ Arty-ROS2, เชื่อม geometry จริงกับ robot model และตรวจ TF/model โดยไม่สร้างค่ากายภาพที่ยังไม่วัดได้

## ความรู้ก่อนเรียน
ROS 2 package, node/topic และโครงสร้าง Arty-ROS2 จากสัปดาห์ 1–10.

## กิจกรรมการเรียนรู้ 100 นาทีต่อเนื่อง
### 1. Engage — 10 นาที
ครูถาม: “ROS 2 จะรู้ได้อย่างไรว่าล้อซ้ายอยู่ด้านไหน ล้อมีขนาดเท่าไร และ sensor อยู่ตรงไหน?” ให้นักเรียนลองวาดหุ่นด้วยกล่อง/วงกลมและตั้งชื่อส่วนต่าง ๆ.

### 2. Explore — 30 นาที
สำรวจ package:
```bash
cd ~/Arty-ROS2/ros2_ws/src/mobile_robot_description
find . -maxdepth 3 -type f -print
```
เปิดไฟล์ Xacro/URDF ที่มีอยู่และค้นคำสำคัญ `link`, `joint`, `origin`, `parent`, `child`, `wheel`.

นักเรียนสร้างตาราง:
| องค์ประกอบ | ประเภท | Parent | Child | หน้าที่ |
|---|---|---|---|---|
| base | link | - | - | ตัวฐาน |
| wheel | link/joint | base | wheel | ล้อขับ |
| caster | fixed support | base | caster | ประคองฐาน |

### 3. Explain — 20 นาที
อธิบาย URDF เป็น robot description, Xacro เป็นวิธีสร้าง/ใช้ parameter และ macro, Joint เชื่อม Link และ TF ใช้บอกความสัมพันธ์ coordinate frames.

Coordinate convention ที่นักเรียนต้องจำ: +X ไปข้างหน้า, +Y ไปทางซ้าย, +Z ขึ้นบน ตาม convention ของ ROS REP-103 ที่ใช้กับ mobile robot.

ล้อขับของโครงการมีเส้นผ่านศูนย์กลาง 65 mm จึงมี `wheel_radius = 0.0325 m`. Caster หน้า/หลังเป็น support และไม่ใช่ drive joint. ค่า wheel separation/base/caster geometry อื่นยังต้องวัดจริง.

### 4. Elaborate — 25 นาที
หลัง build workspace:
```bash
cd ~/Arty-ROS2/ros2_ws
source /opt/ros/jazzy/setup.bash
source install/setup.bash
ros2 pkg prefix mobile_robot_description
```
ให้นักเรียนวาด TF tree ที่คาดหวังจาก model และเปรียบเทียบกับไฟล์จริง. หากเครื่องพร้อม RViz/robot_state_publisher ให้ครูสาธิต model โดยใช้ config ที่ผ่าน validation เท่านั้น.

กิจกรรมวิเคราะห์ข้อผิดพลาด: ครูให้ภาพสมมติล้อวางผิดแกน แล้วให้นักเรียนบอกว่าจะตรวจ `origin xyz/rpy`, parent/child หรือ geometry จุดใด โดยไม่แก้ production source แบบเดาสุ่ม.

### 5. Evaluate — 15 นาที
ส่ง 1) Robot link/joint map 2) TF tree 3) รายการ “ค่าที่ยืนยันแล้ว/ค่าที่ยังต้องวัด” 4) Exit Ticket.

## Safety & Engineering Integrity
URDF ที่ดูสวยไม่ได้แปลว่าค่ากายภาพถูกต้อง. ห้ามเติม wheel separation, base dimensions หรือ caster offsets จากการกะด้วยสายตา; ต้อง Measure → Record → Configure → Validate.

## เกณฑ์
ความรู้ ≥70% • link/joint/frame map ≥80% • แยกค่าจริงกับค่าที่ยังไม่ทราบถูกต้อง 100% • ทำงานกลุ่มระดับ 3/4 ขึ้นไป.
