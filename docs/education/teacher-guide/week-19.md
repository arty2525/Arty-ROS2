# คู่มือครู Week 19 — Mini Challenge

## บทบาทของครู
สัปดาห์นี้ครูเปลี่ยนจากผู้บอกขั้นตอนเป็น **Engineering Coach + Safety Authority**. นักเรียนต้องเลือกวิธีเอง แต่ทุกการทดลองต้องมีเหตุผล หลักฐาน และ stop condition

## วัตถุประสงค์ที่แท้จริง
ภารกิจไม่ได้วัดเพียงว่าหุ่นถึง Goal หรือไม่ แต่ประเมินความสามารถในการวางแผน, predict, test, measure, troubleshoot, document และทำซ้ำผลได้

## การออกแบบสนาม
ครูกำหนดตามพื้นที่จริง โดยมี Start/Goal, ช่วงตรง และการเปลี่ยนทิศอย่างน้อยหนึ่งครั้ง. ไม่ควรตั้ง speed เป็นตัวชี้วัดหลัก. จัด safety boundary และพื้นที่ผู้ชมแยกจาก motion area

## เตรียมก่อนสอน
- ตรวจสนามและพื้นผิว
- ตรวจหุ่นทุกชุด
- เตรียม readiness form/run log
- กำหนด emergency stop procedure
- กำหนดจำนวน run/เวลาต่อทีมให้เท่าเทียม
- เตรียมเครื่องมือวัดระยะ/มุม

## การสอน 100 นาที
### Mission Brief 10 นาที
อธิบายกติกา คะแนน safety และหลักฐานที่ต้องส่ง. แจกบทบาท Driver, Safety Officer, ROS Operator, Data Engineer, Documenter

### Plan 20 นาที
ทีมต้องส่ง prediction/path/command plan/measurement plan ก่อนทดลอง. ครูถาม “ถ้า run นี้ผิด คุณจะรู้จากข้อมูลอะไร?” หากตอบไม่ได้ ให้กลับไปเพิ่ม measurement plan

### Design Review 15 นาที
ตรวจ commit/config/diagnostics/known limitations. อนุมัติ motion เฉพาะทีมที่อธิบาย stop condition ได้

### Test–Measure–Improve 40 นาที
กฎสำคัญ: เปลี่ยนหนึ่งตัวแปรต่อ iteration. ทุก run บันทึก config/goal/result/error/fault/change. ครูไม่ควรรีบบอกวิธีแก้ แต่ถามให้เด็กสร้าง hypothesis

### Evaluate 15 นาที
ให้ทีมเลือก best **reproducible** run ไม่ใช่ run ที่โชคดีที่สุด และอธิบายว่าทำไมผลจึงดีขึ้น

## คำถาม Coaching
- หลักฐานอะไรทำให้คิดว่าสาเหตุอยู่ที่ geometry?
- ถ้าทดลองซ้ำผลเหมือนเดิมหรือไม่?
- ตัวแปรใดเปลี่ยนระหว่างสอง run?
- Diagnostics บอกอะไร?
- ถ้าห้ามปรับ PID คุณจะตรวจอะไรต่อ?

## เกณฑ์ 20 คะแนน
Safety 5 • Engineering process 5 • Performance/repeatability 4 • Technical explanation 3 • Documentation/teamwork 3

## สิ่งที่ครูไม่ควรทำ
อย่าปรับค่าหุ่นให้ทีมโดยตรงเพื่อให้ผ่านภารกิจ; อย่าปล่อยให้หลายคนส่ง command พร้อมกัน; อย่าให้คะแนนเฉพาะความเร็ว; อย่าละเลย failed run เพราะเป็นข้อมูลสำคัญ
