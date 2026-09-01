# สัปดาห์ที่ 17 — Calibration, Encoder Sign และ PID

**เวลา:** 100 นาทีต่อเนื่อง

## จุดประสงค์
นักเรียนสามารถตรวจ motor/encoder direction, ใช้ข้อมูลจริงเพื่อ calibration และอธิบายบทบาท P/I/D โดยไม่ใช้ค่าที่เดาขึ้นเป็น production tuning ได้

## กิจกรรมการเรียนรู้ 100 นาทีต่อเนื่อง
### 1. Engage — 10 นาที
สถานการณ์: สั่งหุ่นเดินตรง แต่หุ่นเบนซ้าย ทั้งที่ command ล้อสองข้างเท่ากัน นักเรียนเสนอสมมติฐานอย่างน้อย 3 ข้อ เช่น wheel geometry, encoder sign, motor response หรือพื้นผิว

### 2. Explore — 25 นาที
ทบทวนหลักฐานจาก Week 08–09 และสร้าง Direction Matrix:
| การทดสอบ | Left motor | Right motor | Left encoder | Right encoder | ผล |
|---|---|---|---|---|---|
| positive command | | | | | |
| forward wheel rotation | | | | | |

ทำการทดสอบบนแท่นโดยล้อพ้นพื้นตาม Safety Gate และเปลี่ยนตัวแปรครั้งละหนึ่งอย่างเท่านั้น

### 3. Explain — 20 นาที
อธิบาย closed-loop control: Setpoint → Error → Controller → Motor → Encoder Feedback. อธิบายเชิงพฤติกรรมว่า P ตอบสนองต่อ error ปัจจุบัน, I สะสม error และ D ตอบสนองต่ออัตราการเปลี่ยน error

เน้นว่า PID tuning เป็นการทดลองกับระบบจริง ไม่ใช้ค่าตัวอย่างเป็นค่าจริง และต้องมี limit/safety ก่อนทดลอง

### 4. Elaborate — 30 นาที
นักเรียนออกแบบ Calibration Experiment โดยเก็บข้อมูล baseline ก่อนทุกครั้ง เช่น setpoint, measured velocity, steady-state error, overshoot/oscillation และเวลาเข้าสู่สภาวะคงที่

ลำดับวิศวกรรม:
1. ยืนยัน geometry/TPR/sign
2. บันทึก baseline
3. ปรับเพียง parameter เดียวตามขั้นตอนครู
4. ทดสอบซ้ำภายใต้เงื่อนไขเดิม
5. เปรียบเทียบ before/after
6. rollback หากพฤติกรรมไม่ปลอดภัย

ไม่กำหนด PID numeric value ในเอกสารนี้ เพราะต้องได้จากการทดลองจริงของหุ่นแต่ละคัน

### 5. Evaluate — 15 นาที
ส่ง Direction Matrix + calibration experiment sheet + graph/table before/after + อธิบาย P/I/D ด้วยภาษาของตนเอง

## Safety Gate
ยกล้อพ้นพื้นในการทดสอบแรก • จำกัด command ตามครูกำหนด • มีผู้ตัดไฟ • ห้าม tuning หาก diagnostics/fault ไม่ปกติ • ห้ามเพิ่ม gain แบบก้าวกระโดดหรือสุ่ม

## เกณฑ์
Direction/sign evidence ครบ • เปลี่ยนตัวแปรทีละหนึ่ง • มี before/after evidence • อธิบาย P/I/D ≥70% • ปฏิบัติปลอดภัย 100%
