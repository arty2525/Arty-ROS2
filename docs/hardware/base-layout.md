# Base Layout

โครงสร้างฐานขับเคลื่อนของ Arty-ROS2 ใช้ Differential Drive

- ล้อขับหลัก 2 ล้อ
- ล้อซ้ายและขวาอยู่บริเวณกึ่งกลางตามแนวยาวของตัวหุ่นยนต์
- เส้นผ่านศูนย์กลางล้อขับหลัก 65 mm
- รัศมีล้อที่ใช้ใน ROS 2 เท่ากับ 0.0325 m
- Caster Ball 1 ลูกด้านหน้า
- Caster Ball 1 ลูกด้านหลัง
- Caster Ball ไม่มี command interface และไม่ถูกใช้คำนวณ odometry

แกนพิกัดของ `base_link`:

- +X = ด้านหน้า
- +Y = ด้านซ้าย
- +Z = ด้านบน

ตำแหน่งล้อขับ:

- `left_wheel_joint`: X = 0, Y = +wheel_separation/2
- `right_wheel_joint`: X = 0, Y = -wheel_separation/2

ตำแหน่ง Caster Ball:

- ด้านหน้า: X = +front_caster_offset
- ด้านหลัง: X = -rear_caster_offset
- Y = 0 ทั้งสองลูก

เพื่อให้ Caster Ball และล้อขับแตะ ground plane เดียวกัน URDF คำนวณตำแหน่ง Z ของศูนย์กลาง Caster Ball จาก:

`caster_z = -wheel_radius + caster_ball_radius`

ค่าที่ยังต้องวัดจากหุ่นยนต์จริง:

- wheel_separation
- wheel_width
- base_length
- base_width
- base_height
- caster_ball_radius
- front_caster_offset
- rear_caster_offset
- ticks_per_revolution

launch จะปฏิเสธค่าศูนย์สำหรับค่าทางกายภาพเหล่านี้ เพื่อไม่ให้ระบบเริ่มด้วย geometry ที่แต่งขึ้น
