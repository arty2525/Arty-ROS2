# คู่มือครู Week 11 — URDF, Xacro และ TF

## เป้าหมายของครู
ครูต้องทำให้นักเรียนเข้าใจว่า ROS 2 ไม่ได้มองหุ่นเป็นเพียงรูป 3 มิติ แต่เป็นโครงสร้างของ **Link + Joint + Coordinate Frame** ที่สัมพันธ์กัน และค่าตำแหน่งในโมเดลต้องสอดคล้องกับฮาร์ดแวร์จริง

## ความรู้ที่ครูควรทบทวนก่อนสอน
**URDF** เป็นรูปแบบ XML สำหรับอธิบายโครงสร้างหุ่นยนต์ เช่น link, joint, geometry, visual, collision และ inertial. **Xacro** ช่วยสร้าง URDF แบบมีตัวแปร macro และการนำไฟล์กลับมาใช้ซ้ำ ลดการเขียนค่าซ้ำหลายจุด. **TF/TF2** ทำหน้าที่ติดตามความสัมพันธ์ของ coordinate frames ตามเวลา

คำสำคัญ:
- `link` = ชิ้นส่วนแข็งของหุ่นในโมเดล
- `joint` = ความสัมพันธ์ระหว่าง parent link และ child link
- `origin xyz` = การเลื่อนตำแหน่ง
- `origin rpy` = การหมุน roll/pitch/yaw
- `fixed joint` = ความสัมพันธ์ที่ไม่มี degree of freedom
- `continuous/revolute joint` = joint ที่หมุนได้ตามข้อกำหนด
- `base_link` = frame หลักที่นิยมใช้อ้างอิงตัวหุ่น

Coordinate convention ที่ควรย้ำ: +X ไปข้างหน้า, +Y ไปทางซ้าย, +Z ขึ้นบน. Rotation ใช้ right-hand rule

## สิ่งที่ต้องเตรียมก่อนเข้าห้อง
1. Build workspace ให้ผ่าน
2. เปิดไฟล์ description/Xacro ของ repository จริงล่วงหน้า
3. ตรวจว่าชื่อ link/joint ในไฟล์ตรงกับ controller configuration
4. เตรียมภาพหุ่นมุมบน/ด้านข้างเพื่อให้นักเรียนเชื่อม physical robot กับ model
5. ห้ามเติม geometry ที่ยังไม่ได้วัดเพื่อให้ภาพดูสมบูรณ์

ค่าที่ยืนยันแล้ว: ล้อขับเส้นผ่านศูนย์กลาง 65 mm จึงมี radius 0.0325 m; ล้อขับอยู่กลางซ้าย/ขวา; caster ball อยู่หน้า/หลังและเป็น support. ค่า wheel separation, base dimensions และ caster offsets ต้องวัดจริง

## ลำดับการสอน 100 นาที
### Engage 10 นาที
ให้เด็กวาดหุ่นเป็นกล่องหนึ่งใบและล้อสองวง แล้วถามว่า “ถ้ากล้องติดอยู่ด้านหน้าหุ่น ROS จะรู้ตำแหน่งกล้องจากตรงไหน?” นำไปสู่แนวคิด frame และ transform

### Explore 30 นาที
ให้นักเรียนสำรวจ package จริง:
```bash
cd ~/Arty-ROS2/ros2_ws/src/mobile_robot_description
find . -maxdepth 3 -type f -print
```
ค้น `link`, `joint`, `parent`, `child`, `origin`. ครูเดินตรวจและถามทุกกลุ่มว่า “joint นี้เชื่อมอะไรกับอะไร?” มากกว่าถามให้จำ syntax

### Explain 20 นาที
วาดต้นไม้บนกระดาน เช่น `base_link → left_wheel`, `base_link → right_wheel`. อธิบายว่า transform ต้องมีเส้นทางเชื่อมกัน หาก frame ขาดจาก tree โปรแกรมอื่นจะไม่สามารถแปลงพิกัดได้

ขยาย Xacro: อธิบายว่าการใช้ตัวแปรเดียวสำหรับ wheel radius ช่วยป้องกันค่าซ้ำไม่ตรงกัน แต่ตัวแปรนั้นต้องมาจากค่าจริง

### Elaborate 25 นาที
ให้นักเรียนเทียบโมเดลกับหุ่นจริง ระบุค่าที่ “Known”, “Measured”, “Unresolved”. หากมี RViz ให้แสดง model และชี้ว่า visual ที่ดูถูกต้องไม่ได้ยืนยันว่า kinematic parameter ถูกต้อง

### Evaluate 15 นาที
ตรวจ TF tree/link-joint map และให้เด็กอธิบายด้วยปากเปล่าว่า parent/child/origin ทำงานอย่างไร

## จุดที่นักเรียนมักสับสน
- คิดว่า Link คือ Node ของ ROS 2 — ไม่ใช่
- คิดว่า TF คือ Topic ธรรมดาเพียงอย่างเดียว — TF2 เป็นระบบจัดการ transform ซึ่งมีการสื่อสารผ่าน ROS
- สลับ +Y ซ้าย/ขวา
- คิดว่า caster ต้องเป็น drive wheel — ไม่ใช่ในฐาน Differential Drive นี้
- เห็น model สวยแล้วคิดว่า dimension ถูกต้อง

## คำถามชี้นำ
**ถาม:** ถ้า wheel radius ในโมเดลผิด จะเกิดอะไร?  
**คาดหวัง:** รูป/การคำนวณที่อาศัยค่าดังกล่าวอาจไม่ตรงหุ่นจริง และถ้าค่าเดียวกันใช้ใน controller จะกระทบระยะ/ความเร็ว

**ถาม:** ทำไมไม่วัด wheel separation จากขอบล้อ?  
**คาดหวัง:** parameter ของ differential drive โดยทั่วไปอ้างระยะระหว่างแนวกึ่งกลางล้อขับ ต้องกำหนด reference ให้ชัด

## Troubleshooting สำหรับครู
Model ไม่ขึ้น → ตรวจ source workspace, package, launch/xacro error.  
ล้ออยู่ผิดด้าน → ตรวจ joint origin และแกน ไม่แก้ด้วยการสลับชื่อแบบสุ่ม.  
TF ขาด → ตรวจ robot_state_publisher, joint state และ parent-child chain

## ขยายความสำหรับนักเรียนเร็ว
ให้อธิบายความต่าง `visual`, `collision`, `inertial` และเหตุใด simulation/physics ต้องการข้อมูลมากกว่า visualization
