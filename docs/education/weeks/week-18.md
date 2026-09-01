# สัปดาห์ที่ 18 — System Integration: รวม Raspberry Pi, ROS 2, ESP32 และฐานขับเคลื่อน

**เวลา:** 100 นาทีต่อเนื่อง

## จุดประสงค์
นักเรียนสามารถ bring up ระบบแบบเป็นชั้น ตรวจ dependency/data flow และใช้หลักฐานเพื่อระบุจุดเสียเมื่อระบบรวมไม่ทำงานได้

## กิจกรรมการเรียนรู้ 100 นาทีต่อเนื่อง
### 1. Engage — 10 นาที
ครูถาม: “ทุกชิ้นทดสอบแยกแล้วผ่าน แต่พอประกอบทั้งระบบหุ่นไม่ขยับ เราควรเริ่มตรวจจากตรงไหน?” นักเรียนห้ามตอบว่า “ลองเปลี่ยนทุกอย่าง” แต่ต้องเสนอลำดับตรวจ

### 2. Explore — 25 นาที
สร้าง Integration Stack:
1. Power/physical wiring
2. ESP32 firmware
3. UART transport
4. ROS 2 hardware interface
5. ros2_control/controller
6. topics/commands
7. odometry/diagnostics

ตรวจ software baseline:
```bash
cd ~/Arty-ROS2
git status
git log -1 --oneline
cd ros2_ws
source /opt/ros/jazzy/setup.bash
source install/setup.bash
ros2 pkg list | grep mobile_robot
```
บันทึก commit SHA ที่ใช้ทุกครั้ง

### 3. Explain — 20 นาที
อธิบาย Integration Testing กับ Unit Testing, dependency chain, observable evidence และเหตุผลที่ต้องตรวจ bottom-up ก่อนส่ง motion command

สร้าง Expected Data Flow:
`cmd_vel → diff_drive_controller → hardware interface → UART → ESP32 → motor driver → wheels`
และ feedback:
`encoder → ESP32 → telemetry → hardware interface → joint states/controller → odometry/diagnostics`

### 4. Elaborate — 30 นาที
ทำ Bring-up Checklist ตามลำดับ โดยแต่ละขั้นต้อง PASS ก่อนขั้นถัดไป:
- [ ] Git/commit baseline ถูกต้อง
- [ ] firmware build/upload ผ่าน
- [ ] physical parameters complete/validated
- [ ] serial device ถูกต้อง
- [ ] UART/heartbeat ปกติ
- [ ] hardware component พร้อม
- [ ] controllers พร้อม
- [ ] `/joint_states` มีข้อมูลสมเหตุผล
- [ ] diagnostics ไม่มี active fault ที่ห้ามเคลื่อนที่
- [ ] motion test บนแท่นผ่าน
- [ ] low-speed floor test ผ่าน

ใช้คำสั่งสำรวจตามระบบจริง:
```bash
ros2 node list
ros2 topic list -t
ros2 control list_controllers
ros2 topic echo /diagnostics
```

### 5. Evaluate — 15 นาที
ครูแจก fault scenario/log ให้แต่ละกลุ่มหนึ่งกรณี เช่น serial device หาย, controller inactive, invalid configuration หรือ encoder sign ผิด ให้วิเคราะห์ layer, evidence, corrective action และ verification

## Safety
ห้ามข้ามขั้นเพื่อให้ทันเวลา • motion test แรกบนแท่น • diagnostics fault ต้องแก้ก่อน floor test • ผู้ควบคุมหนึ่งคน • พื้นที่ทดสอบโล่งและพร้อมตัดไฟ

## ชิ้นงาน
Integration Checklist + System Data Flow + Fault Analysis + commit SHA/log evidence
