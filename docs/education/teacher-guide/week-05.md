# คู่มือครู Week 05 — ROS 2 Graph: Node, Topic และ Message

**เวลา:** 100 นาทีต่อเนื่อง  
**ใช้คู่กับ:** `docs/education/weeks/week-05.md`

## ความรู้ที่ครูควรทบทวน
ROS Graph คือภาพความสัมพันธ์ระหว่างองค์ประกอบที่กำลังทำงาน เช่น Node, Topic, Service และ Action. ในสัปดาห์นี้เน้น Node/Topic/Message/Publisher/Subscriber เพื่อสร้างพื้นฐานก่อนเข้าสู่ package และ controller

- **Node** = โปรแกรม/กระบวนการ ROS 2 หนึ่งหน่วย
- **Publisher** = ผู้ส่ง message ไปยัง topic
- **Subscriber** = ผู้รับ message จาก topic
- **Topic** = ช่องทางชื่อหนึ่งสำหรับ data stream
- **Message type** = โครงสร้างข้อมูลที่ส่งผ่าน topic

ย้ำว่า Topic ไม่ใช่ไฟล์และไม่ใช่สายสัญญาณจริง แต่เป็นชื่อช่องทางสื่อสารใน ROS 2

## การเตรียมก่อนสอน
1. ทดสอบ Talker/Listener ให้ทำงาน
2. ตรวจ `ros2 node list`, `ros2 topic list -t`, `ros2 topic info /chatter`
3. หากมี GUI พร้อม ให้เตรียม `rqt_graph` แต่ไม่จำเป็นต้องใช้เป็นแกนหลัก
4. เตรียมกระดาษ A3 หรือ whiteboard สำหรับวาด graph

## ขั้นตอนสอน 100 นาที
### นาที 0–10: Engage
ถามว่า “ถ้าโปรแกรมควบคุมล้อและโปรแกรมอ่านเซนเซอร์แยกไฟล์กัน แล้วทั้งสองจะแลกข้อมูลกันอย่างไร?” ให้เด็กเสนอแนวคิดก่อนพูดคำว่า Topic

### นาที 10–40: Explore
เปิด Talker แล้วให้นักเรียนใช้คำสั่งสำรวจทีละคำสั่ง:
```bash
ros2 node list
ros2 topic list
ros2 topic list -t
ros2 topic info /chatter
ros2 topic echo /chatter
```
ครูต้องหยุดถามหลังแต่ละคำสั่งว่า “เราเพิ่งรู้ข้อมูลใหม่อะไร?”

### นาที 40–60: Explain
วาด `talker → /chatter → listener` แล้วใส่ `std_msgs/msg/String` กำกับเส้น topic

อธิบายว่า publisher ไม่จำเป็นต้องรู้ implementation ของ subscriber โดยตรง และ subscriber ก็ไม่ต้องเรียกฟังก์ชันใน talker โดยตรง นี่คือ decoupling ซึ่งช่วยให้ระบบหุ่นยนต์ใหญ่ขึ้นได้โดยไม่ผูกทุกส่วนแน่นเกินไป

### นาที 60–85: Elaborate
ใช้:
```bash
ros2 node info /talker
ros2 node info /listener
ros2 interface show std_msgs/msg/String
```
ให้นักเรียนอ่านข้อมูลและวาด graph ใหม่ด้วยตนเอง จากนั้นเชื่อมกับ Arty-ROS2 เช่น `/joint_states`, odometry และ `/diagnostics`

### นาที 85–100: Evaluate
ตรวจ graph, message type และให้นักเรียนอธิบายด้วยคำของตัวเองว่า Node/Topic/Message ต่างกันอย่างไร

## จุดสับสนที่พบบ่อย
**Node = package?** ไม่ใช่ Package เป็นหน่วยจัดโครงสร้าง source/dependency; Node คือโปรแกรมที่กำลังทำงาน

**Topic เก็บข้อมูลไว้ตลอด?** โดยทั่วไป Topic คือ stream ของ message ไม่ใช่ database

**ชื่อ topic เหมือน message type หรือไม่?** ไม่เหมือน ชื่อ topic เช่น `/chatter`; type เช่น `std_msgs/msg/String`

## วิธีอธิบายให้ง่าย
ใช้เปรียบเทียบ “สถานีวิทยุ”: Topic = ช่อง, Publisher = สถานีส่ง, Subscriber = ผู้รับ, Message type = รูปแบบภาษาที่ตกลงกัน แต่ย้ำว่าเป็นเพียง analogy

## Common errors
- `/chatter` ไม่มีใน list → talker อาจไม่รัน
- `echo` เงียบ → publisher ไม่มี message หรือ topic/type ไม่ตรง
- node name ไม่ตรงที่คาด → ให้ใช้ `ros2 node list` ก่อน ไม่เดาชื่อ

## เนื้อหาขยาย
อธิบาย QoS แบบแนวคิดเท่านั้น: ระบบสามารถกำหนดรูปแบบความน่าเชื่อถือ/การเก็บ message ได้ แต่ยังไม่ต้องลงรายละเอียด policy ในสัปดาห์นี้
