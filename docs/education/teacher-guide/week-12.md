# คู่มือครู Week 12 — ros2_control

## แก่นความรู้ที่ครูต้องเข้าใจ
`ros2_control` เป็น framework เชื่อม controller ฝั่ง ROS 2 กับ hardware ผ่านมาตรฐาน interface ไม่ใช่ firmware และไม่ใช่ motor driver

เส้นทางคำสั่งที่ควรเขียนบนกระดาน:
`ROS command → Controller Manager/Controller → Command Interface → Hardware Plugin → UART → ESP32 → Motor Driver → Motor`

เส้นทาง feedback:
`Encoder → ESP32 → UART → Hardware Plugin → State Interface → Controller/Joint State → ROS 2`

## คำศัพท์
- Controller Manager: จัดการ lifecycle และ controller
- Hardware Component/Plugin: ตัวแทน hardware ใน ros2_control
- Command Interface: ค่าที่ controller เขียน เช่น velocity
- State Interface: ค่าที่อ่านจาก hardware เช่น position/velocity
- Joint State Broadcaster: เผยแพร่สถานะ joint
- Diff Drive Controller: แปลงคำสั่งฐานเป็นคำสั่งล้อและคำนวณข้อมูลที่เกี่ยวข้องตาม configuration

## เตรียมก่อนสอน
ครูต้อง build repository จริงและตรวจ package/config/launch. หาก physical parameters ยัง unresolved **อย่าบังคับ activate drive system**; ใช้ source/config/log เพื่อสอน architecture แทน

ตรวจ:
```bash
cd ~/Arty-ROS2/ros2_ws
source /opt/ros/jazzy/setup.bash
colcon build --symlink-install
source install/setup.bash
```

## การสอน 100 นาที
### Engage 10 นาที
ถามว่า `cmd_vel` มี linear/angular velocity แต่ ESP32 ต้องควบคุมล้อ แล้วใครแปลงและส่งค่าระหว่างสองโลกนี้

### Explore 30 นาที
ให้นักเรียนเปิด `mobile_robot_control` และ `mobile_robot_hardware` แล้วจัดไฟล์เป็น 3 กลุ่ม: configuration, launch, hardware implementation. อย่าเริ่มด้วยการอธิบาย code ทุกบรรทัด

### Explain 20 นาที
ใช้ภาพ data flow ด้านบน อธิบาย command/state interface. เน้นว่า controller “ไม่ควรรู้” รายละเอียด UART frame โดยตรง เพราะ hardware layer รับผิดชอบ abstraction

อธิบาย lifecycle แบบง่าย: โหลด → configure → activate → deactivate. การ activate หมายถึงระบบเข้าสู่สถานะที่พร้อมควบคุม จึงต้องมี safety precondition

### Elaborate 25 นาที
หากระบบพร้อม ใช้:
```bash
ros2 control list_controllers
ros2 control list_hardware_components
ros2 control list_hardware_interfaces
ros2 topic echo /joint_states
```
ให้นักเรียนอ่าน output ไม่ใช่แค่ถ่ายภาพ ครูถามว่า controller ใด active, interface ใด available/claimed และ feedback สมเหตุผลหรือไม่

### Evaluate 15 นาที
ให้กลุ่มวาด ros2_control architecture จากความจำ แล้วแจกอาการ “controller ไม่ active” ให้เรียงลำดับตรวจ

## ความสับสนที่ต้องแก้
- `ros2_control` ไม่ใช่ ROS 2 ทั้งระบบ
- Controller ไม่เท่ากับ Motor Driver
- `/joint_states` เป็นสถานะที่เผยแพร่ ไม่ใช่คำสั่งมอเตอร์
- Build ผ่านไม่ได้แปลว่า hardware พร้อม
- Controller active ไม่รับประกันว่าสาย UART/Motor Power ถูกต้อง

## Troubleshooting
`list_controllers` ไม่มีผล → ตรวจ controller_manager/launch.  
Hardware configure ไม่ผ่าน → อ่าน log ของ hardware plugin/config/serial.  
Joint state ค้าง → ไล่ feedback จาก Encoder → ESP32 → UART → read() → state interface.  
ค่ากระโดดผิดปกติ → หยุด motion และตรวจ encoder sign/count/packet validity

## Safety
ครูต้องแยก “software inspection” กับ “motion test”. การตรวจ controller ทำได้ก่อนจ่าย Motor Power หลายส่วน. การ activate drive เพื่อทดลองจริงต้องผ่าน parameter validation และ safety checklist

## เนื้อหาขยาย
ให้นักเรียนอธิบายข้อดีของ hardware abstraction: เปลี่ยน implementation การสื่อสารได้โดย controller ชั้นบนไม่ต้องรู้รายละเอียดทุกอย่าง
