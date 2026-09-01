# สัปดาห์ที่ 1 — รู้จักระบบปฏิบัติการหุ่นยนต์และความปลอดภัย

**รายวิชา:** ระบบปฏิบัติการหุ่นยนต์  
**ระดับชั้น:** ม.4  
**เวลา:** 2 คาบ × 50 นาที = 100 นาที  
**รูปแบบ:** Active Learning 5E + Project-Based Learning + Collaborative Learning

## เป้าหมายการเรียนรู้
นักเรียนสามารถ
1. อธิบายหน้าที่ของ Raspberry Pi, ESP32, Motor Driver, Motor/Encoder และ ROS 2 ได้
2. จำแนกส่วน Compute, Control, Actuator และ Sensor ได้
3. อธิบายหลัก Differential Drive เบื้องต้นได้
4. ปฏิบัติตาม Safety Checklist ก่อนทดลองหุ่นยนต์ได้
5. สร้างแผนภาพสถาปัตยกรรมระบบของ Arty-ROS2 ได้

## อุปกรณ์
- หุ่นยนต์ Arty-ROS2 หรือชุดอุปกรณ์แยกชิ้น
- Raspberry Pi 4
- ESP32 NodeMCU-32S / ESP-WROOM-32
- Cytron MDD10A Rev2.0
- มอเตอร์ Encoder 2 ตัว
- ล้อขับขนาดเส้นผ่านศูนย์กลาง 65 mm
- Caster Ball หน้า/หลัง
- ใบกิจกรรมและ Engineering Logbook

## คาบที่ 1 — 50 นาที

### Engage — 10 นาที
ครูแสดงหุ่นยนต์โดยยังไม่จ่ายกำลังมอเตอร์ และตั้งคำถามว่า “ถ้าต้องทำให้หุ่นยนต์คันนี้รับคำสั่งจาก Linux แล้วขับเคลื่อนได้ เราต้องมีระบบอะไรบ้าง?”

นักเรียนเขียนสิ่งที่คิดลง Logbook โดยยังไม่เปิด source code.

### Explore — 30 นาที
แบ่งกลุ่ม 4–5 คนและหมุนเวียนบทบาท System Engineer, Embedded Engineer, ROS Developer, Safety/Test Engineer และ Documenter.

ให้นักเรียนสำรวจอุปกรณ์และสร้างตาราง:

| อุปกรณ์ | หน้าที่ | รับข้อมูลจาก | ส่งข้อมูลไป |
|---|---|---|---|
| Raspberry Pi 4 | ประมวลผล ROS 2 | ผู้ใช้/ROS nodes | ESP32/ROS network |
| ESP32 | ควบคุมระดับ Embedded | Raspberry Pi/Encoder | Motor Driver/Raspberry Pi |
| MDD10A | ขับกำลังมอเตอร์ | ESP32 | Motor |
| Encoder | วัดการหมุน | เพลามอเตอร์ | ESP32 |

### Explain — 10 นาที
ครูเชื่อมโยงเป็นสายข้อมูลหลัก:

`ผู้ใช้/ROS 2 → Raspberry Pi → UART → ESP32 → PWM/DIR → MDD10A → Motor`

และ feedback:

`Encoder → ESP32 → UART Telemetry → ROS 2 → Odometry/Diagnostics`

## คาบที่ 2 — 50 นาที

### Explain ต่อ — 10 นาที
อธิบาย Differential Drive: ล้อขับซ้ายและขวาควบคุมแยกกัน; Caster Ball หน้า/หลังเป็นล้อประคอง ไม่ใช่ drive joint.

ค่าที่ยืนยันแล้วในโครงการคือ `wheel_radius = 0.0325 m` จากล้อเส้นผ่านศูนย์กลาง 65 mm. ค่าทางกายภาพอื่นต้องวัดจากหุ่นจริง ห้ามเดา.

### Elaborate — 25 นาที
แต่ละกลุ่มวาด System Architecture ของตนเอง โดยต้องมี Compute → Embedded → Driver → Motor และ Feedback จาก Encoder.

จากนั้นทำ Safety Inspection โดย **ยังไม่สั่งมอเตอร์หมุน**.

### Evaluate — 15 นาที
ส่ง:
- แผนภาพระบบ 1 หน้า
- Safety Checklist
- Exit Ticket 3 ข้อ: วันนี้เรียนรู้อะไร / หลักฐานคืออะไร / ยังสงสัยอะไร

## Safety Checklist
- [ ] พื้นที่ทดลองโล่ง
- [ ] ตรวจสายไฟและขั้วต่อ
- [ ] ไม่มีสายแตะชิ้นส่วนหมุน
- [ ] รู้วิธีตัดไฟฉุกเฉิน
- [ ] ระหว่างทดสอบมีผู้สั่งงานเพียงคนเดียว
- [ ] ก่อนทดสอบมอเตอร์ครั้งแรกต้องยกล้อพ้นพื้น
- [ ] ไม่ใช้ค่าพารามิเตอร์ที่เดาขึ้นเอง

## เกณฑ์ประเมิน
- ความรู้/Exit Ticket ≥ 70%
- ขั้นตอนปฏิบัติถูกต้อง ≥ 80%
- Safety และ Teamwork ระดับ 3 จาก 4 ขึ้นไป
- แผนภาพต้องแสดง data flow และ feedback ได้ถูกต้อง

## งานเตรียมสัปดาห์ถัดไป
นักเรียนเตรียม Raspberry Pi/Ubuntu หรือเครื่อง Linux สำหรับฝึก Terminal และทบทวนคำศัพท์ `terminal`, `shell`, `directory`, `package`, `permission`, `SSH`.
