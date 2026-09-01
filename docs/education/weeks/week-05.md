# สัปดาห์ที่ 5 — ROS 2 Graph: Node, Topic และ Message

**เวลา:** 100 นาทีต่อเนื่อง

## จุดประสงค์
อธิบาย Node/Topic/Publisher/Subscriber, ใช้ ROS 2 CLI สำรวจ graph, อ่าน message type และสร้าง data-flow diagram ได้

## กิจกรรมการเรียนรู้ 100 นาทีต่อเนื่อง
### 1. Engage — 10 นาที
ถามว่าหากโปรแกรมควบคุมล้อ โปรแกรม sensor และโปรแกรมแสดงผลแยกกัน จะแลกข้อมูลกันอย่างไร.

### 2. Explore — 30 นาที
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
บันทึก node/topic/type และ publisher/subscriber.

### 3. Explain — 20 นาที
สรุป ROS Graph, message interface และ Topic ในฐานะช่องทางเชิงตรรกะ. ทดลอง `ros2 interface show std_msgs/msg/String`.

### 4. Elaborate — 25 นาที
เปิด listener แล้วใช้:
```bash
ros2 node info /talker
ros2 node info /listener
```
วาด `talker → /chatter → listener` พร้อม message type และเชื่อมโยงกับ `/joint_states`, odometry, diagnostics ของ Arty-ROS2 โดยยังไม่สั่งหุ่นเคลื่อนที่.

### 5. Evaluate — 15 นาที
ส่ง ROS Graph + Node/Topic/Type table + Exit Ticket. ความรู้ ≥70%, CLI ≥80%, graph ต้องแสดงทิศข้อมูลถูกต้อง.
