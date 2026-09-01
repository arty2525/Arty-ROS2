# คู่มือครู Week 13 — Differential Drive และ cmd_vel

## ความรู้ที่ครูควรทบทวน
ฐาน Differential Drive ใช้ล้อขับซ้าย/ขวาที่ควบคุมความเร็วแยกกัน. ถ้าความเร็วเท่ากันและทิศเดียวกัน ฐานเคลื่อนตรง; ถ้าต่างกันฐานจะโค้ง; ถ้าขนาดเท่ากันแต่ทิศตรงข้าม ฐานหมุนใกล้จุดกึ่งกลาง

สำหรับความเร็วฐาน `v` และความเร็วเชิงมุม `ω` หากระยะกึ่งกลางล้อซ้ายถึงขวาเป็น `L`:

`v_left = v - ωL/2`  
`v_right = v + ωL/2`

ถ้าต้องการ angular wheel speed และ wheel radius คือ `r`:
`ω_left_wheel = v_left/r`, `ω_right_wheel = v_right/r`

ใช้สูตรเพื่ออธิบายแนวคิด ไม่แทนค่า `L` จนกว่าจะวัดจริง. `r = 0.0325 m` เป็นค่าที่ยืนยันแล้ว

## Twist ที่ครูต้องอธิบาย
สำหรับฐานบนพื้นราบ เราสนใจหลัก ๆ `linear.x` และ `angular.z`. `geometry_msgs/msg/Twist` มี vector linear และ angular ครบสามแกน แต่ไม่ได้หมายความว่าหุ่น differential drive เคลื่อนที่อิสระทุกแกนได้

## เตรียมก่อนสอน
- ตรวจ wheel radius/separation/sign/TPR
- ตรวจ diagnostics/controller
- ทำ motion test บนแท่นก่อนนักเรียน
- กำหนด speed limit สำหรับห้องเรียนตามระบบจริง
- ทำเครื่องหมาย forward direction บนฐาน

## การสอน 100 นาที
### Engage 10 นาที
ใช้ล้อจำลองสองวง ให้นักเรียนทำนายการเคลื่อนที่ 4 แบบก่อนเห็นสูตร

### Explore 25 นาที
ให้นักเรียนสร้างตาราง left/right wheel: +/+, -/-, -/+, +/-, และกรณี magnitude ไม่เท่ากัน แล้ววาด trajectory

### Explain 20 นาที
จึงค่อยแนะนำ `v`, `ω`, `L`, `r` และสูตร. เน้นหน่วย m/s และ rad/s. อธิบายว่า `angular.z > 0` ตาม right-hand rule หมายถึงหมุนทวนเข็มเมื่อมองจาก +Z ลงมา

### Elaborate 30 นาที
ตรวจ message:
```bash
ros2 interface show geometry_msgs/msg/Twist
ros2 topic list -t
```
ให้ค้น topic ของ controller จากระบบจริง ไม่ให้จำชื่อ topic จากเอกสารตัวอย่าง. ก่อน command ทุกครั้ง นักเรียนต้องเขียน Prediction แล้วจึง Test และ Record

### Evaluate 15 นาที
ให้โจทย์เชิงเหตุผล เช่น “ต้องการเลี้ยวซ้ายขณะเดินหน้า ล้อใดควรมีความเร็วเชิงเส้นมากกว่า?” และให้เชื่อมคำตอบกับสูตร

## จุดสับสน
- สับสน angular velocity ของฐานกับ angular velocity ของล้อ
- ใช้ diameter แทน radius
- ใช้ mm ในสูตรที่คาดหน่วย m
- เข้าใจว่า caster เป็นล้อที่ controller ต้องสั่ง
- คิดว่าค่า command บวกหมายถึง motor electrical polarity แบบเดียวกันทั้งสองข้างเสมอ ทั้งที่ mounting/sign convention ต้องยืนยันจริง

## Troubleshooting
สั่ง forward แต่หมุน → ตรวจ motor/encoder sign และ mapping ซ้ายขวา.  
เดินตรงแต่เบน → ตรวจ geometry, wheel diameter, response/calibration, พื้นผิว.  
หมุนมุมผิดมาก → ตรวจ wheel separation และ encoder/odometry ก่อนปรับ PID

## Safety
Prediction ก่อน Motion • test แรกยกล้อ • low speed • clear area • one operator • stop immediately เมื่อทิศไม่ตรง prediction
