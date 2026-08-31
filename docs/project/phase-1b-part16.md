# Phase 1B Part 16

หัวข้อ: Physical Base Geometry

ปรับแบบจำลองฐานให้ตรงกับฮาร์ดแวร์จริง:

- ล้อขับหลัก 2 ล้ออยู่กึ่งกลางซ้าย/ขวา
- ล้อขับเส้นผ่านศูนย์กลาง 65 mm
- `wheel_radius = 0.0325 m`
- Caster Ball ด้านหน้า 1 ลูก
- Caster Ball ด้านหลัง 1 ลูก
- Caster Ball เป็น fixed support link ใน URDF และไม่เข้า ros2_control
- เพิ่ม parameter สำหรับรัศมีและระยะติดตั้ง Caster Ball โดยไม่สร้างค่าคาดเดา
