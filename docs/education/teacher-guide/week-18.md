# คู่มือครู Week 18 — System Integration

## เป้าหมายสำหรับครู
ฝึกนักเรียนให้รวมระบบแบบมีลำดับ ไม่ใช่เปิดทุกอย่างพร้อมกันแล้วแก้จากอาการรวม

## Integration Stack ที่ครูควรใช้
1. Mechanical/Power/Wiring
2. ESP32 firmware
3. Encoder/Motor local behavior
4. UART transport/protocol
5. ROS hardware interface
6. ros2_control/controller
7. ROS topics/commands
8. Odometry/diagnostics

หลักสำคัญ: **ชั้นล่างต้องมีหลักฐาน PASS ก่อนพึ่งพาชั้นบน**

## เตรียมก่อนสอน
ครูต้องมี known-good system หนึ่งชุดหรืออย่างน้อย known-good logs, บันทึก commit SHA, config version, firmware version และ serial device. เตรียม fault scenario ที่ปลอดภัย เช่น controller inactive/config unresolved/log timeout

## การสอน 100 นาที
### Engage 10 นาที
ให้สถานการณ์ “ทุก component เคยผ่าน แต่รวมแล้วไม่วิ่ง” ถามว่าจะตรวจอะไรเป็นอันดับแรกและเพราะอะไร

### Explore 25 นาที
นักเรียนสร้าง dependency map ของ Arty-ROS2 และทำ readiness inspection โดยยังไม่ motion test

คำสั่งสำรวจ:
```bash
cd ~/Arty-ROS2
git status
git log -1 --oneline
cd ros2_ws
source /opt/ros/jazzy/setup.bash
source install/setup.bash
ros2 node list
ros2 topic list -t
ros2 control list_controllers
```

### Explain 20 นาที
อธิบาย unit test, integration test และ end-to-end test. ย้ำว่า end-to-end fail ไม่บอกทันทีว่าชั้นไหนเสีย จึงต้องใช้ observability/diagnostics แบ่งระบบ

### Elaborate 30 นาที
ทำ Bring-up ทีละ gate: firmware → serial → hardware → controller → feedback → diagnostics → raised-wheel motion → low-speed floor test. นักเรียนต้องเซ็น PASS/FAIL และ evidence ทุก gate

หาก FAIL ห้ามข้ามไปขั้นต่อไปเพียงเพราะ “อยากลองดู”

### Evaluate 15 นาที
แจก fault scenario ต่างกันแต่ละกลุ่ม ให้ระบุ failing layer, evidence ที่ต้องขอ, hypothesis, test และ recovery

## จุดสับสน
- Build success ≠ runtime success
- Topic มีชื่ออยู่ ≠ มีข้อมูลถูกต้อง
- Controller active ≠ motor power/serial/encoder ถูก
- Restart ≠ root-cause correction
- Git HEAD/config ไม่ตรงกันทำให้ผลทดลองเปรียบเทียบไม่ได้

## Troubleshooting Pattern
Power → Device → Firmware → Transport → Hardware Plugin → Controller → Topic → Physical response → Feedback

ครูควรห้ามนักเรียนกระโดดไปแก้ PID หาก UART ยังไม่ stable หรือ encoder sign ยังไม่ถูก

## Safety
ระบบรวมมีความเสี่ยงสูงกว่าทดสอบ component. Motion gate ต้องมีพื้นที่โล่ง, ล้อพ้นพื้นครั้งแรก, one operator, stop path และ diagnostics normal
