# สัปดาห์ที่ 4 — ติดตั้งและตรวจสอบ ROS 2 Jazzy

**เวลา:** 100 นาที

## เป้าหมาย
นักเรียนตรวจ Ubuntu/architecture, source ROS environment และทดสอบ publisher/subscriber ได้.

> เครื่องหลักของโครงการใช้ Ubuntu 24.04 LTS 64-bit และ ROS 2 Jazzy. ครูควรเตรียม installation ไว้ล่วงหน้าเพื่อไม่ให้เวลาเรียนสูญเสียจากการดาวน์โหลดพร้อมกันทั้งห้อง.

## คาบที่ 1
### Engage — 10 นาที
ครูให้ดูคำสั่ง `ros2` บนเครื่องที่ source environment แล้วเทียบกับ shell ที่ยังไม่ source เพื่อชวนคิดเรื่อง environment.

### Explore — 30 นาที
ตรวจระบบ:

```bash
uname -m
cat /etc/os-release
locale
```

หากเครื่องติดตั้ง ROS 2 แล้ว:

```bash
source /opt/ros/jazzy/setup.bash
printenv ROS_DISTRO
ros2 --help
```

ผลที่ต้องตรวจคือ `ROS_DISTRO=jazzy`.

### Explain — 10 นาที
อธิบาย distribution, workspace overlay และเหตุผลที่ต้อง source setup file.

## คาบที่ 2
### Explore/Elaborate — 30 นาที
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

ให้นักเรียนระบุว่าใครเป็น publisher, subscriber และข้อมูลเดินทางอย่างไร.

### Explain — 5 นาที
ครูเชื่อมโยง demo กับหุ่นจริง: node ควบคุม → topic → controller/driver และ sensor/encoder → topic → node ประมวลผล.

### Evaluate — 15 นาที
ส่ง Talker/Listener log และตอบ:
1. ROS_DISTRO คืออะไร?
2. ทำไมเปิด terminal ใหม่แล้วอาจต้อง source อีกครั้ง?
3. ถ้า listener ไม่ได้รับข้อมูล จะตรวจอะไรเป็น 3 อย่างแรก?

## ชิ้นงาน
- System verification log
- Talker/Listener evidence
- Exit Ticket

## สำหรับครู
คู่มือติดตั้งเต็มอยู่ที่ `docs/education/installation-guide.md`. ก่อนสอนให้ใช้เครื่อง/SD card image ที่ผ่านการทดสอบแล้วเป็น baseline ของห้องเรียน.
