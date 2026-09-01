# สัปดาห์ที่ 5 — ROS 2 Graph: Node, Topic และ Message

**รายวิชา:** ระบบปฏิบัติการหุ่นยนต์ • ม.4  
**เวลา:** 2 คาบ × 50 นาที

## จุดประสงค์
นักเรียนสามารถ 1) อธิบาย Node/Topic/Publisher/Subscriber 2) ใช้ ROS 2 CLI สำรวจ graph 3) อ่านชนิด message และ 4) สร้างแผนภาพการไหลของข้อมูลได้

## คาบที่ 1
### Engage — 10 นาที
ครูถาม: ถ้าหุ่นมีโปรแกรมควบคุมล้อ โปรแกรมอ่านเซนเซอร์ และโปรแกรมแสดงผล โปรแกรมเหล่านี้จะแลกข้อมูลกันอย่างไรโดยไม่รวมทุกอย่างไว้ไฟล์เดียว?

### Explore — 30 นาที
Terminal A:
```bash
source /opt/ros/jazzy/setup.bash
ros2 run demo_nodes_cpp talker
```
Terminal B:
```bash
source /opt/ros/jazzy/setup.bash
ros2 node list
ros2 topic list
ros2 topic list -t
ros2 topic info /chatter
ros2 topic echo /chatter
```
นักเรียนบันทึกชื่อ node, topic, type และความสัมพันธ์ publisher/subscriber.

### Explain — 10 นาที
สรุป ROS Graph และแนวคิด message interface. เน้นว่า Topic คือช่องทางเชิงตรรกะ ไม่ใช่สายไฟจริง.

## คาบที่ 2
### Explain — 10 นาที
ทดลองดู interface:
```bash
ros2 interface show std_msgs/msg/String
```

### Elaborate — 25 นาที
เปิด Listener แล้วใช้:
```bash
ros2 node info /talker
ros2 node info /listener
```
ให้นักเรียนวาด graph `talker → /chatter → listener` พร้อมระบุ message type.

จากนั้นเชื่อมโยงกับ Arty-ROS2 เช่น `/joint_states`, odometry และ diagnostics โดยยังไม่ต้องสั่งหุ่นเคลื่อนที่.

### Evaluate — 15 นาที
ชิ้นงาน: ROS Graph 1 ภาพ + ตาราง Node/Topic/Type + Exit Ticket.

## คำถาม
1. Node กับ Topic ต่างกันอย่างไร?
2. ทำไม subscriber ต้องทราบ message type?
3. ถ้า topic มีอยู่แต่ `echo` ไม่มีข้อมูล ควรตรวจอะไรต่อ?

## เกณฑ์ผ่าน
ความรู้ ≥70%, CLI task ≥80%, graph แสดงทิศข้อมูลถูกต้อง และบันทึกหลักฐานครบ.
