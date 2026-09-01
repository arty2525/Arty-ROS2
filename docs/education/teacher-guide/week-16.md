# คู่มือครู Week 16 — การวัดพารามิเตอร์จริง

## จุดประสงค์ของคู่มือครู
ทำให้ครูสามารถกำกับการวัด geometry อย่างมีมาตรฐาน เพราะค่าที่คลาดเคลื่อนจะส่งผลต่อ model, kinematics และ odometry

## ความรู้ที่ควรทบทวน
**Accuracy** คือความใกล้ค่าจริง, **Precision** คือความสม่ำเสมอของการวัดซ้ำ, **Resolution** คือความละเอียดเครื่องมือ และ **Uncertainty** คือขอบเขตความไม่แน่นอนที่ควรรายงาน

ROS ใช้ SI units เป็นหลัก จึงควรบันทึกค่าปลายทางเป็นเมตร ไม่ผสม mm/cm/m ใน config โดยไม่แปลง

## Reference ที่ต้องกำหนดก่อนวัด
- wheel radius: จาก geometry ล้อจริง; โครงการยืนยัน 0.0325 m
- wheel separation: ระยะระหว่างแนวกึ่งกลางล้อขับซ้ายและขวาตาม geometry ที่ controller ใช้
- base dimensions: กำหนดขอบอ้างอิงให้ชัด
- caster offset: ต้องระบุว่าอ้างจาก frame/จุดใด

หากแต่ละกลุ่มวัดคนละ reference point ค่าเฉลี่ยรวมไม่มีความหมาย

## เตรียมก่อนสอน
ตรวจไม้บรรทัด/เวอร์เนียร์, ทำเครื่องหมาย reference, เตรียมแบบบันทึก, ปิด Motor Power และวางหุ่นให้มั่นคง

## การสอน 100 นาที
### Engage 10 นาที
ให้เด็กสองกลุ่มวัด “ความกว้างหุ่น” โดยไม่บอก reference แล้วเปรียบเทียบผล เพื่อแสดงว่าคำจำกัดความสำคัญพอ ๆ กับเครื่องมือ

### Explore 25 นาที
ครูสาธิตการอ่าน scale และการมองตั้งฉากเพื่อลด parallax. นักเรียนวัดแต่ละ parameter อย่างน้อย 3 ครั้ง โดยผู้วัดมากกว่าหนึ่งคน

### Explain 20 นาที
อธิบาย mean, spread และเหตุผลที่ต้องบันทึก raw data. ห้ามสอนให้ลบค่าที่ไม่สวยโดยไม่มีเหตุผล; outlier ต้องมีหลักฐานว่าเกิดจากวิธีวัดผิด

### Elaborate 30 นาที
ให้กลุ่มสร้าง Physical Parameter Record: parameter, definition, tool, raw trials, mean, unit, uncertainty/remark, ผู้วัด, วันที่ และภาพจุดวัด

จากนั้นใช้ validator ของโครงการเพื่อตรวจสถานะ config แต่ยังไม่แก้ production config จนผ่าน review

### Evaluate 15 นาที
ครูสุ่มเลือก parameter แล้วให้นักเรียนอีกกลุ่มทำ repeat measurement. คะแนนเน้น reproducibility และ traceability ไม่ใช่การได้เลขเหมือนครู

## จุดสับสน
- วัด wheel separation จากขอบนอกแทน centerline
- แปลง 65 mm เป็น radius 0.065 m ซึ่งผิด; 65 mm คือ diameter จึง radius 32.5 mm = 0.0325 m
- ใส่หน่วย mm ลงช่องที่ software คาด m
- ใช้ค่าเฉลี่ยจากการวัดคนละนิยาม

## หลักฐานที่ครูต้องเก็บ
Raw data + ภาพ reference + calculation + approved value. เมื่อนำค่าเข้า config ต้องสามารถย้อนกลับมาหาเอกสารนี้ได้
