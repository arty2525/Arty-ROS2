# คู่มือครู Week 17 — Calibration และ PID เบื้องต้น

## สิ่งที่ครูต้องเข้าใจก่อนสอน
Calibration คือการทำให้ค่าที่ระบบใช้สอดคล้องกับพฤติกรรมจริงโดยอาศัยการวัด ไม่ใช่การปรับเลขจนผลดูดี. PID เป็น feedback controller รูปแบบหนึ่ง แต่ก่อน tuning ต้องแก้ geometry, encoder TPR/sign, wiring และ mechanical issue ให้ถูกก่อน

## PID เชิงแนวคิด
Error `e(t) = setpoint - measured value`

- **P** ตอบสนองตาม error ปัจจุบัน: มากขึ้นมักตอบสนองแรงขึ้น แต่สูงเกินอาจสั่น
- **I** สะสม error ตามเวลา: ช่วยลด steady-state error แต่สะสมมากอาจเกิด windup
- **D** ตอบสนองต่ออัตราการเปลี่ยน error: ช่วย damping บางกรณีแต่ไวต่อ noise

ไม่กำหนดค่า gain ตัวเลขในคู่มือ เพราะต้องได้จากระบบจริง

## ลำดับก่อน PID ที่ครูต้องบังคับ
1. ตรวจ wheel geometry
2. ตรวจ encoder TPR
3. ตรวจ motor direction
4. ตรวจ encoder sign
5. ตรวจ feedback stability
6. ตรวจ command limit/fail-safe
7. เก็บ baseline
8. จึงเริ่ม tuning

## การสอน 100 นาที
### Engage 10 นาที
แสดงกราฟสมมติ 3 แบบ: ช้าแต่ถึงเป้า, overshoot, oscillation. ให้นักเรียนบรรยายพฤติกรรมก่อนใช้คำศัพท์ PID

### Explore 25 นาที
สร้าง Direction Matrix จากการทดลองจริงบนแท่น. ครูให้เด็กทำนาย sign ก่อนหมุน แล้วบันทึก command, physical direction และ encoder response

### Explain 20 นาที
วาด closed loop: setpoint → error → controller → plant/motor → sensor → feedback. อธิบาย P/I/D ด้วยพฤติกรรม ไม่เริ่มจาก calculus สำหรับระดับ ม.4

### Elaborate 30 นาที
ให้นักเรียนทำ controlled experiment: เก็บ baseline, เปลี่ยน parameter เพียงตัวเดียวตามขั้นตอนครู, ใช้ command profile เดิม, เก็บ measured response และเปรียบเทียบ rise/settling/overshoot/steady-state behavior

ทุกครั้งต้องมี rollback value และ stop condition

### Evaluate 15 นาที
ให้เด็กดูกราฟที่ไม่เคยเห็นและอธิบายว่าอาการใดควรตรวจ mechanical/measurement ก่อน และอาการใดอาจเกี่ยวกับ controller

## จุดสับสน
- หุ่นเบนซ้ายไม่ได้แปลว่าเพิ่ม PID ฝั่งซ้ายทันที
- ค่า gain มากไม่ได้แปลว่าดี
- Tuning โดยเปลี่ยน P/I/D พร้อมกันทำให้สรุปสาเหตุไม่ได้
- Encoder noise อาจทำให้ derivative term มีปัญหา
- Calibration กับ tuning เป็นคนละแนวคิด แม้เกี่ยวข้องกัน

## Safety
ล้อพ้นพื้นในการทดลองแรก, command limit, clear area, operator คนเดียว, diagnostics ปกติ, stop เมื่อ oscillation/ทิศผิด/เสียงหรือความร้อนผิดปกติ

## สำหรับนักเรียนที่เรียนเร็ว
อธิบาย integral windup, derivative noise และเหตุผลที่ระบบจริงมักมี saturation/filter/anti-windup แต่ไม่ให้ปรับ production controller โดยไม่มี test plan
