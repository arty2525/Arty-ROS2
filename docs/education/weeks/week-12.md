# สัปดาห์ที่ 12 — ros2_control: เชื่อม ROS 2 กับ Hardware

**เวลา:** 100 นาทีต่อเนื่อง

## จุดประสงค์
นักเรียนอธิบาย hardware interface, controller manager, state/command interface และ joint state broadcaster รวมทั้งตรวจ controller/joint state อย่างเป็นระบบได้

## กิจกรรมการเรียนรู้ 100 นาทีต่อเนื่อง
### 1. Engage — 10 นาที
ถาม: `cmd_vel` เป็นความเร็วของหุ่น แต่ Motor Driver ต้องการคำสั่งระดับมอเตอร์ แล้วใครทำหน้าที่เชื่อม software กับ hardware?

### 2. Explore — 30 นาที
สำรวจ package:
```bash
cd ~/Arty-ROS2/ros2_ws/src
find mobile_robot_control mobile_robot_hardware -maxdepth 3 -type f -print
```
ค้นใน config/launch/plugin ว่ามี `controller_manager`, `joint_state_broadcaster`, `diff_drive_controller` และ hardware plugin อยู่ตรงไหน.

หลัง build:
```bash
cd ~/Arty-ROS2/ros2_ws
source /opt/ros/jazzy/setup.bash
source install/setup.bash
ros2 pkg prefix mobile_robot_control
ros2 pkg prefix mobile_robot_hardware
```

### 3. Explain — 20 นาที
ครูอธิบาย data path:
`Controller → command interface → Hardware Interface → UART → ESP32`
และ feedback:
`ESP32 telemetry → Hardware Interface → state interface → Controller/Joint States`.

อธิบาย lifecycle/configure/activate เชิงแนวคิด และเหตุผลที่ configuration ที่ไม่สมบูรณ์ควร fail ก่อนทำให้มอเตอร์เคลื่อนที่.

### 4. Elaborate — 25 นาที
เมื่อหุ่นมีค่ากายภาพ/encoder ที่วัดครบและครูอนุญาต จึงใช้เครื่องมือ ROS 2 control ตรวจระบบ เช่น:
```bash
ros2 control list_controllers
ros2 control list_hardware_components
ros2 control list_hardware_interfaces
ros2 topic echo /joint_states
```
หาก config ยังไม่ครบ ให้ทำกิจกรรมจาก output/log ที่ครูเตรียมแทนการใส่ค่าปลอม.

นักเรียนสร้าง troubleshooting tree: controller ไม่ active → ตรวจ controller manager → hardware state → serial → ESP32 → fault/config.

### 5. Evaluate — 15 นาที
ส่ง ros2_control architecture + controller status evidence หรือ supplied-log analysis + troubleshooting tree + Exit Ticket.

## Safety Gate
ก่อน activate drive controller ต้องยืนยัน physical parameters, encoder TPR, inversion และพื้นที่ทดสอบ. หาก parameter validator ระบุ unresolved value ให้หยุดและวัด ไม่ bypass validation.

## เกณฑ์
อธิบาย command/state path ถูกต้อง ≥80% • troubleshooting มีอย่างน้อย 4 ชั้น • Safety gate ผ่านครบก่อน hardware test.
