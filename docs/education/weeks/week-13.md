# สัปดาห์ที่ 13 — Differential Drive และ cmd_vel

**เวลา:** 100 นาทีต่อเนื่อง

## จุดประสงค์
นักเรียนอธิบาย linear/angular velocity, วิเคราะห์ความเร็วล้อซ้าย/ขวา และส่ง `geometry_msgs/msg/Twist` อย่างปลอดภัยเมื่อระบบพร้อมได้

## กิจกรรมการเรียนรู้ 100 นาทีต่อเนื่อง
### 1. Engage — 10 นาที
ให้กลุ่มใช้เหรียญ/วงกลมแทนล้อสองข้าง แล้วตอบว่า “ถ้าล้อสองข้างหมุนเท่ากันจะเกิดอะไร? ถ้าข้างหนึ่งเร็วกว่า? ถ้าหมุนสวนกัน?”

### 2. Explore — 25 นาที
วิเคราะห์ 4 กรณีโดยยังไม่ใช้มอเตอร์: เดินหน้า, ถอยหลัง, เลี้ยวโค้ง, หมุนอยู่กับที่. ให้นักเรียนเขียนเครื่องหมายความเร็วล้อซ้าย/ขวาและทิศการหมุนของฐาน.

### 3. Explain — 20 นาที
อธิบาย `Twist`: `linear.x` คือความเร็วเดินหน้าของฐาน และ `angular.z` คือความเร็วหมุนรอบแกน Z. เชื่อมกับ diff-drive kinematics เชิงแนวคิด: ความเร็วฐานถูกแปลงเป็นความเร็วล้อโดยอาศัย wheel radius และ wheel separation.

เน้นว่า wheel radius ยืนยันแล้ว 0.0325 m แต่ wheel separation ต้องใช้ค่าที่วัดจากศูนย์กลางล้อจริง.

### 4. Elaborate — 30 นาที
ตรวจชนิด message:
```bash
source /opt/ros/jazzy/setup.bash
ros2 interface show geometry_msgs/msg/Twist
```
เมื่อระบบผ่าน Safety Gate และ controller พร้อม ครูอาจให้ทดสอบ command ความเร็วต่ำตามค่าที่ครูกำหนดผ่าน topic ของ diff_drive_controller ที่ใช้งานจริง โดยต้องตรวจชื่อ topic จาก:
```bash
ros2 topic list -t
```
ห้ามคัดลอกชื่อ topic จากตัวอย่างโดยไม่ตรวจระบบจริง.

นักเรียนทำ prediction ก่อนส่งทุก command แล้วเปรียบเทียบ Actual vs Predicted.

### 5. Evaluate — 15 นาที
ส่งตาราง 4 กรณี: linear.x, angular.z, แนวโน้ม left/right wheel, predicted motion, actual motion (ถ้าทดสอบจริง) และอธิบายผลของ wheel separation ต่อการหมุน.

## Safety Gate
ล้อพ้นพื้นในการทดสอบแรก • diagnostics ปกติ • ผู้ควบคุมหนึ่งคน • เริ่มความเร็วต่ำ • พร้อมหยุดทันที • ไม่ทดสอบหาก TPR/separation/inversion ยังไม่ยืนยัน.
