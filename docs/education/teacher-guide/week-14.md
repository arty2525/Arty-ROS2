# คู่มือครู Week 14 — Odometry

## แนวคิดหลักสำหรับครู
Odometry ของ differential drive ประมาณการการเปลี่ยน pose จากการหมุนของล้อ. เป็น **การประมาณ** ไม่ใช่ตำแหน่งจริงสัมบูรณ์ และ error สะสมตามเวลา

ลำดับคำนวณที่ควรสอน:
`encoder counts → wheel revolutions/angle → wheel distance → Δleft/Δright → Δdistance/Δyaw → pose`

ถ้า TPR คือจำนวน count ต่อรอบ output shaft:
`revolutions = Δticks / TPR`

ระยะล้อ:
`distance = revolutions × 2πr`

สำหรับการเคลื่อนช่วงสั้น:
`Δs = (Δs_right + Δs_left)/2`  
`Δθ = (Δs_right - Δs_left)/L`

สูตรขึ้นกับ sign/convention ที่ถูกต้อง จึงต้องยืนยัน encoder direction ก่อน

## Frame ที่ต้องอธิบาย
`odom` เป็น frame อ้างอิงต่อเนื่องสำหรับ local odometry; `base_link` เป็น frame ของตัวหุ่น. อย่าสอนว่า odom คือ GPS หรือพิกัดโลกที่ไม่มี drift

## เตรียมก่อนสอน
วัด TPR/radius/separation จริง, เตรียมพื้นที่เส้นตรงและเครื่องมือวัด, ตรวจ encoder/diagnostics, เก็บ dataset สำรองกรณี hardware ใช้ไม่ได้

## การสอน 100 นาที
### Engage 10 นาที
ถาม “ถ้าปิดกล้องและ GPS หุ่นจะรู้ได้อย่างไรว่าขยับไปเท่าไร?” ให้เด็กเสนอ sensor ที่ใช้ได้

### Explore 25 นาที
ใช้ข้อมูล encoder จาก Week 09 ให้เด็กแปลงทีละขั้น โดยเขียนหน่วยกำกับทุกบรรทัด. ครูตรวจว่าพวกเขาไม่สับสน ticks กับ revolutions

### Explain 20 นาที
สาธิตกรณี Δleft = Δright → Δθ ≈ 0 และกรณีต่างกัน → yaw เปลี่ยน. อธิบาย systematic error กับ random/slip error

### Elaborate 30 นาที
ค้น topic จริง:
```bash
ros2 topic list -t | grep -E 'odom|joint_states'
```
อ่าน odometry topic ที่พบและเปรียบเทียบกับระยะวัดจริง. ทำหลาย trial โดยเงื่อนไขเหมือนกัน

คำนวณ `error = robot estimate - physical measurement` และ `%error = |error|/physical measurement × 100`

### Evaluate 15 นาที
ให้นักเรียนแยกสาเหตุ error เป็น parameter error, encoder/sign error, mechanical/slip และ measurement error พร้อมเสนอวิธีตรวจทีละข้อ

## จุดสับสน
- Odometry ไม่ใช่ ground truth
- ความแม่นระยะตรงไม่ได้แปลว่ามุมหมุนแม่น
- wheel radius ผิดมีผลต่อ scale ระยะ
- wheel separation ผิดเด่นชัดต่อ yaw
- PID ไม่ใช่คำตอบแรกของ odometry error ทุกชนิด

## Troubleshooting
ระยะ scale ผิดสม่ำเสมอ → ตรวจ TPR/radius.  
หมุนผิดมุม → ตรวจ separation/sign.  
ค่า jump → ตรวจ count rollover/parser/packet/encoder.  
แต่ละ trial กระจายมาก → ตรวจ slip, mechanical play, surface และวิธีวัด

## ขยายความ
อธิบายว่าระบบนำทางจริงมัก fuse odometry กับ IMU/LiDAR/vision เพื่อจำกัด drift แต่ Week นี้เน้น wheel odometry ให้เข้าใจฐานก่อน
