# สัปดาห์ที่ 1 — รู้จักระบบปฏิบัติการหุ่นยนต์และความปลอดภัย

**รายวิชา:** ระบบปฏิบัติการหุ่นยนต์ • **ระดับชั้น:** ม.4 • **เวลา:** 100 นาทีต่อเนื่อง  
**รูปแบบ:** Active Learning 5E + Project-Based Learning + Collaborative Learning

## เป้าหมายการเรียนรู้
นักเรียนสามารถ 1) อธิบายหน้าที่ Raspberry Pi, ESP32, Motor Driver, Motor/Encoder และ ROS 2 2) จำแนก Compute/Control/Actuator/Sensor 3) อธิบาย Differential Drive เบื้องต้น 4) ใช้ Safety Checklist และ 5) สร้าง System Architecture ของ Arty-ROS2 ได้

## อุปกรณ์
Raspberry Pi 4, ESP32 NodeMCU-32S/ESP-WROOM-32, Cytron MDD10A Rev2.0, Encoder Motor 2 ตัว, ล้อขับ 65 mm, Caster Ball หน้า/หลัง, ใบกิจกรรมและ Engineering Logbook

## กิจกรรมการเรียนรู้ 100 นาทีต่อเนื่อง
### 1. Engage — 10 นาที
ครูแสดงหุ่นยนต์โดยยังไม่จ่ายกำลังมอเตอร์และถาม “ถ้าต้องทำให้หุ่นยนต์รับคำสั่งจาก Linux แล้วขับเคลื่อนได้ เราต้องมีระบบอะไรบ้าง?” นักเรียนเขียนสิ่งที่รู้และสิ่งที่สงสัยลง Logbook.

### 2. Explore — 30 นาที
แบ่งกลุ่ม 4–5 คน หมุนเวียนบทบาท System Engineer, Embedded Engineer, ROS Developer, Safety/Test Engineer และ Documenter. สำรวจอุปกรณ์และสร้างตารางหน้าที่/ข้อมูลเข้า/ข้อมูลออก.

| อุปกรณ์ | หน้าที่ | รับข้อมูลจาก | ส่งข้อมูลไป |
|---|---|---|---|
| Raspberry Pi 4 | ประมวลผล ROS 2 | ผู้ใช้/ROS nodes | ESP32/ROS network |
| ESP32 | Embedded control | Pi/Encoder | Driver/Pi |
| MDD10A | ขับกำลังมอเตอร์ | ESP32 | Motor |
| Encoder | วัดการหมุน | เพลามอเตอร์ | ESP32 |

### 3. Explain — 20 นาที
ครูเชื่อมโยง data flow: `ผู้ใช้/ROS 2 → Raspberry Pi → UART → ESP32 → PWM/DIR → MDD10A → Motor` และ feedback: `Encoder → ESP32 → UART Telemetry → ROS 2 → Odometry/Diagnostics`.

อธิบาย Differential Drive: ล้อขับซ้าย/ขวาควบคุมแยกกัน; Caster Ball หน้า/หลังเป็นล้อประคอง ไม่ใช่ drive joint. ค่าที่ยืนยันคือ `wheel_radius = 0.0325 m`; ค่าอื่นต้องวัดจริง ห้ามเดา.

### 4. Elaborate — 25 นาที
แต่ละกลุ่มวาด System Architecture ที่มี Compute → Embedded → Driver → Motor และ Encoder Feedback จากนั้นทำ Safety Inspection โดยยังไม่สั่งมอเตอร์หมุน.

### 5. Evaluate — 15 นาที
ส่ง System Architecture 1 หน้า + Safety Checklist + Exit Ticket: วันนี้เรียนรู้อะไร / หลักฐานคืออะไร / ยังสงสัยอะไร.

## Safety Checklist
- [ ] พื้นที่ทดลองโล่ง
- [ ] ตรวจสายไฟและขั้วต่อ
- [ ] ไม่มีสายแตะชิ้นส่วนหมุน
- [ ] รู้วิธีตัดไฟฉุกเฉิน
- [ ] มีผู้สั่งงานเพียงคนเดียวขณะทดสอบ
- [ ] ทดสอบมอเตอร์ครั้งแรกต้องยกล้อพ้นพื้น
- [ ] ไม่ใช้ค่าพารามิเตอร์ที่เดาขึ้นเอง

## เกณฑ์ประเมิน
ความรู้ ≥70% • ปฏิบัติ ≥80% • Safety/Teamwork ระดับ 3/4 ขึ้นไป • แผนภาพ data flow และ feedback ถูกต้อง
