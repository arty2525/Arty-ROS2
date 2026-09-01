# สัปดาห์ที่ 14 — Odometry: จาก Encoder สู่ตำแหน่งหุ่นยนต์

**เวลา:** 100 นาทีต่อเนื่อง

## จุดประสงค์
นักเรียนอธิบายเส้นทาง Encoder → wheel rotation → distance → pose, อ่าน odometry และคำนวณ error จากการทดลองจริงได้

## กิจกรรมการเรียนรู้ 100 นาทีต่อเนื่อง
### 1. Engage — 10 นาที
ครูถาม: “หุ่นไม่มี GPS ในห้องเรียน แล้วรู้ได้อย่างไรว่าตัวเองเคลื่อนที่ไปประมาณ 1 เมตร?”

### 2. Explore — 25 นาที
ใช้ข้อมูล tick ที่ได้จาก Week 09 และ wheel radius 0.0325 m. นักเรียนเขียนกระบวนการคำนวณโดยไม่ต้องแทน TPR จนกว่าจะมีค่าที่วัดจริง:
`Δticks → revolutions → wheel angle/distance → left/right displacement → robot pose`.

### 3. Explain — 20 นาที
อธิบาย circumference `2πr`, การแปลง tick เป็นรอบ, ความต่างระยะล้อซ้าย/ขวากับการเปลี่ยน yaw, frame `odom` และ `base_link`, accumulated error และ wheel slip.

### 4. Elaborate — 30 นาที
เมื่อระบบพร้อม:
```bash
ros2 topic list -t | grep -E 'odom|joint_states'
```
เลือก odometry topic ที่ระบบจริงประกาศและใช้ `ros2 topic echo <ODOM_TOPIC>`.

ทดลองในพื้นที่วัดระยะ: ทำจุดเริ่ม → สั่งเดินทางระยะที่ครูกำหนดด้วยความเร็วต่ำ → วัดระยะจริง → บันทึก odometry → คำนวณ
`error = measured_by_robot - physical_measurement`
และ `%error = |error| / physical_measurement × 100`.

หากระบบยังไม่พร้อม ให้ใช้ dataset จริงที่ครูเก็บจากหุ่นแทน.

### 5. Evaluate — 15 นาที
ส่ง calculation sheet + ตารางอย่างน้อย 3 trials + ค่า error + วิเคราะห์สาเหตุอย่างน้อย 3 ข้อ เช่น TPR, radius, separation, slip, timing หรือ inversion.

## Engineering Integrity
ห้ามแก้ parameter เพื่อให้ผล “ตรง 1 เมตร” โดยไม่มีวิธี calibration ที่อธิบายได้. ต้องเก็บ before/after data ทุกครั้ง.
