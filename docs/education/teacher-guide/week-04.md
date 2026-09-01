# คู่มือครู Week 04 — ROS 2 Jazzy

**เวลา:** 100 นาทีต่อเนื่อง  
**ใช้คู่กับ:** `docs/education/weeks/week-04.md`

## สิ่งที่ครูควรรู้ก่อนสอน
ROS 2 ไม่ใช่ระบบปฏิบัติการ แต่เป็น middleware, libraries และเครื่องมือสำหรับพัฒนาหุ่นยนต์ โดยโครงการนี้ใช้ Ubuntu 24.04 LTS เป็นระบบปฏิบัติการและ ROS 2 Jazzy เป็น ROS distribution หลัก

คำสำคัญที่ครูควรอธิบายได้ ได้แก่ ROS distribution, environment, setup file, node, publisher, subscriber, underlay และ overlay

## ทำไมต้อง source setup file
`source /opt/ros/jazzy/setup.bash` โหลด environment ของ ROS 2 เข้าสู่ shell ปัจจุบัน ทำให้ shell รู้จักคำสั่งและ package ของ ROS 2 ดังนั้น terminal ใหม่อาจต้อง source ใหม่

ควรหลีกเลี่ยงการพูดว่า “source เพื่อเปิด ROS” เพราะจะทำให้นักเรียนเข้าใจว่า ROS เป็นโปรแกรมเดียว ควรอธิบายว่าเป็นการเตรียม environment ของ shell

## การเตรียมก่อนเข้าห้อง
1. ตรวจ Ubuntu version ด้วย `cat /etc/os-release`
2. ตรวจ architecture ด้วย `uname -m`
3. ตรวจว่ามี `/opt/ros/jazzy/setup.bash`
4. ทดลอง `ros2 --help`
5. ทดลอง Talker/Listener ด้วยตนเองก่อนสอน
6. หากใช้หลายเครื่อง ให้เริ่มสาธิตบนเครื่องเดียวก่อน เพื่อลดตัวแปรเรื่อง network

## ขั้นตอนสอน 100 นาที
### นาที 0–10: Engage
เปิด terminal แล้วแสดงความแตกต่างระหว่าง shell ที่เตรียม ROS environment แล้วกับ shell ที่ยังไม่ได้เตรียม จากนั้นถามนักเรียนว่า “โปรแกรมหายไปจริงหรือ shell ยังไม่รู้จัก?”

### นาที 10–40: Explore
ให้นักเรียนตรวจระบบทีละคำสั่ง:
```bash
uname -m
cat /etc/os-release
locale
source /opt/ros/jazzy/setup.bash
printenv ROS_DISTRO
ros2 --help
```
อธิบายผลทุกคำสั่ง ไม่ให้ copy แล้วผ่านไปทันที

### นาที 40–60: Explain
วาดภาพ `Ubuntu → ROS 2 Jazzy → Arty-ROS2 Workspace` และอธิบายว่าหลัง build workspace จะมี environment ของ workspace เพิ่มอีกชั้น

### นาที 60–85: Elaborate
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
ให้นักเรียนสังเกตผู้ส่ง ผู้รับ และข้อมูลที่ไหลระหว่างสองโปรแกรม

### นาที 85–100: Evaluate
ถามนักเรียนรายบุคคล:
1. ROS 2 เป็น OS หรือไม่
2. `source` ทำอะไร
3. ทำไม terminal ใหม่อาจต้อง source อีกครั้ง
4. Talker/Listener แสดงแนวคิดอะไร

## จุดที่นักเรียนมักสับสน
- Ubuntu กับ ROS 2 เป็นคนละชั้นของระบบ
- `source` มีผลกับ shell ปัจจุบัน
- package ของ workspace จะยังไม่ถูกค้นพบถ้ายังไม่ได้ build/source overlay
- Talker/Listener เป็นตัวอย่าง publisher/subscriber ไม่ใช่โครงสร้างทั้งหมดของ ROS 2

## Common errors
**`ros2: command not found`**: ตรวจว่าติดตั้ง ROS แล้วและ source ถูกต้อง  
**package not found**: ตรวจ package installation หรือ workspace overlay  
**listener ไม่มีข้อมูล**: เริ่มตรวจ talker, environment และทำให้ตัวอย่างบนเครื่องเดียวทำงานก่อน

## วิธีช่วยนักเรียนแก้ปัญหา
ใช้ลำดับคงที่: ตรวจ OS → ตรวจ setup file → source → ตรวจ `ROS_DISTRO` → ตรวจ `ros2 --help` → run node ทีละตัว

ไม่ควรเริ่มจากการติดตั้งใหม่ทันที เพราะจะทำให้นักเรียนไม่เรียนรู้การวิเคราะห์สาเหตุ

## เนื้อหาขยาย
อธิบาย underlay/overlay และตั้งคำถามนำเข้าสัปดาห์ 5 ว่า “Talker และ Listener เชื่อมกันผ่านโครงสร้างอะไรใน ROS 2?”
