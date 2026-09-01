# คู่มือครู Week 15 — Diagnostics, Fault Handling และ Fail-safe

## แก่นที่ครูต้องถ่ายทอด
การแก้ปัญหาหุ่นยนต์ต้องเริ่มจาก **หลักฐาน** ไม่ใช่การสุ่มเปลี่ยน code/สาย/parameter. Diagnostics ทำให้ health state ของระบบสังเกตได้ และ fail-safe คือการออกแบบให้ความผิดปกตินำระบบไปสู่สภาวะที่ลดความเสี่ยง

Fault ที่หลักสูตรใช้ทบทวน: command timeout, heartbeat timeout, invalid configuration, protocol error, invalid velocity command และ unknown fault bits หากมี

## คำศัพท์
- Symptom = อาการที่เห็น
- Root cause = สาเหตุราก
- Fault = เงื่อนไขผิดปกติที่ระบบตรวจพบ
- Diagnostic = ข้อมูลบอกสถานะ/สุขภาพ
- Timeout = เกินเวลาที่กำหนดโดยไม่มีเหตุการณ์ที่คาด
- Fail-safe = พฤติกรรมลดความเสี่ยงเมื่อผิดปกติ
- Recovery = ขั้นตอนคืนระบบหลังแก้สาเหตุ

## เตรียมก่อนสอน
เตรียม diagnostic log ทั้งกรณีปกติและผิดปกติ. อย่าสร้าง fault ทางไฟฟ้าที่เสี่ยง. หากต้องการสาธิต communication timeout ให้ใช้วิธี software/test harness ที่ปลอดภัยหรือ log ที่บันทึกไว้

## การสอน 100 นาที
### Engage 10 นาที
สถานการณ์ UART หยุดระหว่างวิ่ง ถามเด็กว่า “คำสั่งมอเตอร์เดิมควรอยู่ต่อไปหรือไม่?” นำเข้าสู่ watchdog/timeout

### Explore 25 นาที
```bash
ros2 topic list -t | grep diagnostic
ros2 topic echo /diagnostics
```
ให้นักเรียนหา level/message/key-value และเขียน “สิ่งที่เห็น” แยกจาก “สิ่งที่คิดว่าเป็นสาเหตุ”

### Explain 20 นาที
วาด health chain: ROS → hardware interface → serial/UART → firmware → motor control. อธิบายว่า fault ในชั้นหนึ่งอาจแสดง symptom อีกชั้นหนึ่ง เช่นไม่มี odometryเพราะ serial telemetry หาย

### Elaborate 30 นาที
แจก fault scenario ให้กลุ่มกรอกตาราง Symptom → Evidence → Hypothesis → Test → Root Cause → Corrective Action → Verification. กฎคือหนึ่ง hypothesis ต้องมีวิธีพิสูจน์/หักล้าง

### Evaluate 15 นาที
ให้กลุ่มอธิบาย troubleshooting flow โดยห้ามใช้คำว่า “ลองเปลี่ยนดู” หากไม่มีเหตุผล

## จุดสับสน
- Error message ไม่จำเป็นต้องเป็น root cause เสมอ
- Clear fault โดยไม่แก้สาเหตุไม่ถือว่าแก้สำเร็จ
- Restart ทุกอย่างอาจทำให้อาการหายชั่วคราวแต่ทำลายหลักฐาน
- Diagnostics ปกติไม่ได้พิสูจน์ mechanical system ทุกส่วน

## ขั้นตอนมาตรฐานที่ครูควรใช้ทุกครั้ง
1. STOP motion
2. Preserve evidence
3. Read diagnostics/log
4. Identify layer
5. Form hypothesis
6. Test one hypothesis
7. Correct root cause
8. Re-test
9. Confirm recovery
10. Record

## Safety
ห้าม short circuit, reverse polarity, stall motor หรือดึงสายไฟกำลังเพื่อสร้าง fault. การเรียน fault handling ต้องไม่สร้างอันตรายใหม่
