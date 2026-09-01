# คู่มือครู Week 20 — Final Demonstration, Post-test และ Reflection

## เป้าหมายสำหรับครู
ประเมินทั้งความรู้ ความสามารถปฏิบัติ กระบวนการวิศวกรรม ความปลอดภัย และความเข้าใจรายบุคคล ไม่ให้ Final Demo กลายเป็นเพียงการดูว่าหุ่นวิ่งได้หรือไม่ได้

## เตรียมก่อนสอน
1. Final rubric และคะแนนรายบุคคล/รายทีม
2. Post-test ที่ครอบคลุม 20 สัปดาห์
3. Safety/Readiness card
4. ลำดับเวลาสาธิตแต่ละทีม
5. คำถาม Engineering Viva หลายระดับ
6. แบบ Reflection
7. วิธีสำรองหลักฐานกรณี hardware ขัดข้อง เช่น log/video/data จาก run ที่ตรวจสอบได้

## การสอน 100 นาที
### Readiness 10 นาที
ตรวจ commit SHA, configuration status, diagnostics, known limitations และ stop procedure. ทีมที่ safety ไม่ผ่านต้องแก้ก่อนสาธิต

### Post-test 20 นาที
ข้อสอบควรวัดความเข้าใจ ไม่ใช่จำ command เช่น ให้ output แล้วถามความหมาย, ให้ fault แล้วเลือกลำดับตรวจ, ให้ diagram แล้วระบุ data flow

### Final Demonstration 40 นาที
ทีมต้องอธิบาย architecture ก่อน motion. จากนั้นแสดง diagnostics, mission, encoder/odometry evidence และ troubleshooting case. ครูให้คะแนน “อธิบายได้ว่าทำไม” ควบคู่กับ “ทำได้”

### Engineering Viva 15 นาที
สุ่มสมาชิกตอบรายบุคคล เพื่อป้องกันกรณีคนหนึ่งทำทั้งหมด. ตัวอย่าง:
- ROS 2 กับ ESP32 แบ่งหน้าที่อย่างไร?
- ถ้า encoder sign ผิด odometry จะเป็นอย่างไร?
- ถ้า heartbeat หาย ระบบควรทำอะไร?
- wheel separation มีผลต่ออะไร?
- ทำไม Git commit SHA สำคัญต่อการทดลอง?

### Reflection 15 นาที
ให้นักเรียนเทียบตัวเอง Week 01 กับ Week 20 โดยอ้างหลักฐานจริง เช่น terminal log, measurement, graph, fault analysis หรือ Git history

## Rubric 100 คะแนน
Safety/Engineering discipline 20 • ROS 2/System Architecture 20 • Hardware/Embedded/Communication 15 • Control/Odometry/Diagnostics 15 • Demonstration/repeatability 15 • Evidence-based troubleshooting 10 • Documentation/teamwork 5

## แนวทางตรวจอย่างยุติธรรม
หาก hardware เสียโดยไม่ใช่ความผิดของนักเรียน ไม่ควรให้คะแนนเป็นศูนย์ทั้งงาน. ประเมินจาก readiness, evidence, diagnosis และความสามารถอธิบายระบบ. แต่หากไม่ผ่านเพราะละเลย safety/configuration ที่เคยกำหนด ต้องสะท้อนใน rubric

## สิ่งที่นักเรียนควรตอบได้เมื่อจบรายวิชา
อธิบายครบเส้นทาง `ROS 2 command → Controller → Hardware Interface → UART → ESP32 → Motor Driver → Motor` และ feedback `Encoder → ESP32 → UART → Hardware Interface → ROS 2 → Odometry/Diagnostics`

พร้อมอธิบายหลัก **Measure → Configure → Test → Record → Improve** และเหตุผลว่าทำไม “ห้ามเดาค่าฮาร์ดแวร์”

## หลังจบการสอน
ครูรวบรวม common misconception, parameter/installation issue และเวลาที่ใช้จริงในแต่ละกิจกรรม เพื่อปรับ revision ของหลักสูตรรุ่นถัดไปโดยเก็บ version ผ่าน Git
