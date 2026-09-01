# วิชาระบบปฏิบัติการหุ่นยนต์ - ม.4

กลุ่มสาระการเรียนรู้วิทยาศาสตร์และเทคโนโลยี

เอกสารชุดนี้จัดโครงสร้างสำหรับการสอน 20 สัปดาห์ สัปดาห์ละ 1 วัน วันละ 2 คาบ คาบละ 50 นาที รวม 40 คาบ โดยใช้ Arty-ROS2 เป็นแพลตฟอร์มการเรียนรู้แบบลงมือปฏิบัติจริง

## เส้นทางการเรียนรู้

1. ความปลอดภัยและสถาปัตยกรรมหุ่นยนต์
2. Ubuntu/Linux และ SSH
3. Git/GitHub และการจัดการ Source Code
4. ROS 2 Jazzy: Node, Topic, Message และ Workspace
5. ESP32 และ PlatformIO
6. Motor Driver, PWM/DIR และ Encoder
7. UART ระหว่าง Raspberry Pi กับ ESP32
8. URDF/Xacro และ TF
9. ros2_control และ Differential Drive
10. Odometry, Diagnostics และ Fault Handling
11. การวัดพารามิเตอร์จริงและ Calibration
12. System Integration, Mini Challenge และการนำเสนอ

## โครงสร้างเอกสาร

- `installation-guide.md` - คู่มือติดตั้งสำหรับครูและนักเรียน
- `course-map-20-weeks.md` - แผนภาพรวม 20 สัปดาห์
- `lesson-plans/` - แผนการสอนรายสัปดาห์ (เพิ่ม/ปรับปรุงต่อเนื่อง)
- `assets/` - ภาพและสื่อประกอบการเรียนรู้

## หลักสำคัญในการสอน

- ใช้ Active Learning 5E + Project-Based Learning + Collaborative Learning
- นักเรียนทำงานเป็นทีมและหมุนเวียนบทบาท System Engineer, Embedded Engineer, ROS Developer, Test/Safety Engineer และ Documenter
- ทุกการทดลองต้องมีหลักฐาน เช่น Terminal log, ตารางข้อมูล, ภาพวงจร, Git diff หรือ Engineering Logbook
- ห้ามสร้างค่าฮาร์ดแวร์ขึ้นเอง ค่าที่ไม่ทราบต้องวัดจากหุ่นจริงก่อนนำไปใช้
- การทดสอบมอเตอร์ครั้งแรกต้องยกล้อพ้นพื้น เริ่มด้วยความเร็วต่ำ และมีวิธีตัดไฟฉุกเฉิน

## แพลตฟอร์ม

- Raspberry Pi 4
- Ubuntu 24.04 LTS 64-bit ARM
- ROS 2 Jazzy
- ESP32 NodeMCU-32S / ESP-WROOM-32
- Cytron MDD10A Rev2.0
- 2 x Cytron planetary encoder motor TG42E-24K
- ล้อขับเส้นผ่านศูนย์กลาง 65 mm (`wheel_radius = 0.0325 m`)
- Caster Ball ด้านหน้าและด้านหลัง

> Repository นี้เป็นระบบที่กำลังพัฒนา ควรใช้ commit/tag เดียวกันทุกกลุ่มในชั้นเรียน และตรวจ GitHub Actions ก่อนเริ่มกิจกรรมปฏิบัติ
