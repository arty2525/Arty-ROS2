# สัปดาห์ที่ 19 — Mini Challenge: ภารกิจหุ่นยนต์เคลื่อนที่

**เวลา:** 100 นาทีต่อเนื่อง

## เป้าหมาย
ประยุกต์ Linux, Git, ROS 2, Embedded, Encoder, Differential Drive, Odometry และ Diagnostics เพื่อวางแผน ทดสอบ และปรับปรุงภารกิจจริงแบบทีมวิศวกรรม

## ภารกิจ
ให้หุ่นยนต์เคลื่อนที่ในสนามที่ครูกำหนดจาก Start ไปยัง Goal โดยมีช่วงเดินตรงและเปลี่ยนทิศทางอย่างน้อยหนึ่งครั้ง ทีมต้องเน้น **ความปลอดภัย ความทำซ้ำได้ และหลักฐานทางวิศวกรรม** มากกว่าความเร็ว

> ขนาดสนาม ระยะทาง และ speed limit ให้ครูกำหนดจากพื้นที่/หุ่นจริง ไม่กำหนดตัวเลขตายตัวใน repository

## กิจกรรมการเรียนรู้ 100 นาทีต่อเนื่อง
### 1. Engage / Mission Brief — 10 นาที
ครูเปิดโจทย์ ขอบเขตสนาม Safety Zone กติกาหยุดฉุกเฉิน และเกณฑ์คะแนน แต่ละทีมกำหนด Driver, Safety Officer, ROS Operator, Data Engineer และ Documenter

### 2. Explore / Plan — 20 นาที
ทีมตรวจ readiness และเขียนแผนก่อนเปิด Motor Power:
- commit SHA
- physical/config validation
- controller/diagnostics state
- predicted path
- command sequence
- measurement points
- stop condition

### 3. Explain / Design Review — 15 นาที
แต่ละทีมอธิบายต่อครูว่า command จะเดินผ่านระบบอย่างไรและ feedback ใดใช้ตัดสินความสำเร็จ ครูอนุมัติ Safety Gate ก่อนทดลอง

### 4. Elaborate / Test–Measure–Improve — 40 นาที
ทำรอบทดลองแบบ engineering iteration:
`Predict → Test → Measure → Record → Change ONE variable → Re-test`

ทุก run ต้องบันทึก:
| Run | Commit/Config | เป้าหมาย | ผลจริง | Error | สิ่งที่เปลี่ยน | Fault/Safety |
|---|---|---|---|---|---|---|

ห้ามปรับหลาย parameter พร้อมกัน และห้ามซ่อน run ที่ล้มเหลว เพราะ failure data เป็นหลักฐานการเรียนรู้

### 5. Evaluate — 15 นาที
ทีมสาธิต best reproducible run และส่ง Engineering Evidence Pack: readiness checklist, run table, odometry/diagnostic evidence, ปัญหาที่พบ, การแก้ และสิ่งที่จะปรับก่อน Final Demonstration

## Rubric 20 คะแนน
- Safety 5
- Systematic engineering process 5
- Mission performance/repeatability 4
- ROS/robot understanding 3
- Documentation/teamwork 3

หากฝ่าฝืน Safety Gate ครูหยุด run ได้ทันทีและต้องแก้ก่อนเริ่มใหม่
