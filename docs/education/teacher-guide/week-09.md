# คู่มือครู Week 09 — Encoder และการวัด Ticks per Revolution

**เวลา:** 100 นาทีต่อเนื่อง  
**ใช้คู่กับ:** `docs/education/weeks/week-09.md`

## ความรู้ที่ครูควรทบทวน
Encoder แบบ quadrature มีสัญญาณ A/B ที่เหลื่อมเฟสกัน ทำให้ระบบสามารถนับการเคลื่อนที่และแยกทิศทางได้ จำนวน count ที่ firmware เห็นต่อการหมุนหนึ่งรอบอาจไม่เท่ากับตัวเลขที่นักเรียนอ่านจาก datasheet โดยตรง เพราะขึ้นกับจุดที่วัด, gear ratio, edge counting และนิยามใน code

สิ่งสำคัญคือโครงการต้องการค่า **counts/ticks ต่อหนึ่งรอบของ output shaft ที่ระบบ firmware ใช้จริง** ไม่ใช่การเดาจากสเปก

## ความหมายที่ครูควรอธิบาย
- Tick/Count: หน่วยการนับ event จาก encoder
- Channel A/B: สัญญาณสองเฟสสำหรับนับและบอกทิศ
- TPR/CPR/PPR: คำใน datasheet อาจนิยามต่างกัน ต้องอ่านให้ชัด
- Gear ratio: จำนวนรอบ motor shaft ต่อ output shaft
- Sign: เครื่องหมายบวก/ลบของ count ตาม direction convention

## การเตรียมก่อนสอน
1. ตรวจ GPIO Encoder: Left 32/33, Right 34/35
2. ยืนยันว่า GPIO34/35 ไม่มี internal pull-up
3. ตรวจชนิด output/ระดับแรงดันของ encoder ก่อนใช้งาน
4. เตรียมวิธีดู encoder count จาก firmware/telemetry ที่ผ่านการตรวจแล้ว
5. ปิด Motor Power ถ้าจะหมุนล้อด้วยมือ
6. ทำจุด reference บนล้อหรือเพลา

## ขั้นตอนสอน 100 นาที
### นาที 0–10: Engage
ถามว่า “ถ้าสั่ง PWM 50% เรารู้ได้ไหมว่าล้อหมุนกี่รอบ?” พาเด็กไปสู่แนวคิดว่า command ไม่ใช่ measurement

### นาที 10–35: Explore
ให้หมุนล้อด้วยมือและดู count เปลี่ยน ถ้าเครื่องมือพร้อม ให้ทดลองหมุนไปข้างหน้า/ย้อนกลับและสังเกตเครื่องหมาย

ครูถาม:
- ทำไม count เพิ่ม/ลด?
- ถ้าหมุนล้อกลับทิศ count ควรเกิดอะไร?
- ถ้าหมุนแล้ว count ไม่เปลี่ยน จะตรวจอะไร?

### นาที 35–55: Explain
วาด waveform A/B แบบง่ายเพื่ออธิบาย phase relationship ไม่จำเป็นต้องลง logic table เต็มในครั้งแรก

ย้ำคำว่า **output shaft revolution** เพราะมอเตอร์แบบมีเกียร์มีหลายจุดที่เรียกว่า “หนึ่งรอบ” ได้

### นาที 55–85: Elaborate — วัด TPR จริง
ลำดับมาตรฐาน:
1. ทำ reference mark
2. บันทึก initial count
3. หมุน output shaft จำนวนรอบที่กำหนดช้า ๆ
4. บันทึก final count
5. คำนวณ `abs(final-initial)/จำนวนรอบ`
6. ทำซ้ำอย่างน้อย 3 ครั้ง
7. เปรียบเทียบค่าซ้าย/ขวา

แนะนำให้หมุนหลายรอบต่อ trial เพื่อลด error จากการพลาดตำแหน่งเริ่ม/จบหนึ่ง tick แต่จำนวนรอบจริงให้ครูกำหนดตามอุปกรณ์

### นาที 85–100: Evaluate
ตรวจ measurement table, ค่าเฉลี่ย, วิธีวัด และคำอธิบาย source of error

## จุดสับสนที่พบบ่อย
**PPR ใน datasheet = TPR ใน firmware เสมอ?** ไม่เสมอ ต้องดูนิยามและ counting method

**เกียร์เพิ่ม torque แล้ว encoder count ไม่เกี่ยว?** ถ้า encoder อยู่ก่อน gear ratio จำนวน pulse ต่อ output revolution จะถูกคูณด้วยอัตราทด

**count ลบ = encoder เสีย?** ไม่ใช่ เครื่องหมายลบอาจเพียงแสดงทิศตาม convention

## Common problems
- count ไม่เปลี่ยน → ตรวจ wiring, power ของ encoder, signal level, pin mapping, firmware interrupt/input
- count กระโดด/ผิดปกติ → ตรวจ noise, ground, signal conditioning/pull-up ตามชนิด output และ mechanical connection
- ซ้าย/ขวา sign ต่างกัน → อาจเกิดจาก orientation ของ encoder ต้องเทียบกับ forward convention ก่อนเลือก inversion

## หลัก Engineering Integrity
ห้ามใช้ค่า TPR ที่ดูเหมือน “นิยมใช้กัน” ในอินเทอร์เน็ต ตัวเลขนี้มีผลโดยตรงต่อ velocity/odometry และต้องย้อนกลับถึงการทดลองจริงได้

## เนื้อหาขยาย
อธิบาย resolution กับ accuracy ว่าคนละเรื่อง Encoder resolution สูงไม่ได้รับประกันว่า odometry แม่น หาก radius/separation/slip ผิด
