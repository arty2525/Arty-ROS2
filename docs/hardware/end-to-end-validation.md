# End-to-End Hardware Validation

เอกสารนี้ใช้ตรวจระบบ Raspberry Pi 4 → UART → ESP32 → Cytron MDD10A → Motor/Encoder → ros2_control ก่อนเริ่ม Phase ถัดไป

## เงื่อนไขก่อนเริ่ม

- ล้อยกพ้นพื้น
- มีสวิตช์ตัดไฟมอเตอร์ที่เข้าถึงได้ทันที
- Raspberry Pi และ ESP32 ใช้กราวด์ร่วมกัน
- UART ใช้ `/dev/serial0` และ 115200 baud
- ค่า `wheel_radius`, `wheel_separation`, `ticks_per_revolution` เป็นค่าที่วัดจริง
- inversion ทั้ง 4 ค่าได้รับการตรวจทีละล้อ

## Gate 1: Build

```bash
cd ~/Arty-ROS2
./scripts/verify_phase1b_part15.sh
```

ต้อง build firmware และ ROS 2 packages ผ่านทั้งหมด

## Gate 2: UART

```bash
ls -l /dev/serial0
```

ผู้ใช้ที่รัน ROS 2 ต้องมีสิทธิ์อ่าน/เขียน serial device

## Gate 3: Bringup

```bash
source /opt/ros/jazzy/setup.bash
source ~/Arty-ROS2/ros2_ws/install/setup.bash
ros2 launch mobile_robot_bringup robot.launch.py
```

ระบบต้องไม่ผ่าน launch หาก physical calibration ยังเป็นศูนย์

## Gate 4: Joint Mapping

ระบบยอมรับเฉพาะ `left_wheel_joint` และ `right_wheel_joint` และไม่ใช้ลำดับใน URDF เป็นตัวตัดสินว่าล้อใดเป็นซ้ายหรือขวา

## Gate 5: Encoder Sign

หมุนล้อไปข้างหน้าด้วยมือแล้วตรวจ:

```bash
ros2 topic echo /joint_states
```

position ของล้อนั้นต้องเพิ่มขึ้น หากลดลงให้เปลี่ยนเฉพาะ encoder inversion ของล้อนั้น

## Gate 6: Motor Sign

สั่งความเร็วต่ำขณะลอยล้อและตรวจทีละข้าง คำสั่งบวกต้องทำให้ล้อหมุนในทิศเดินหน้า หากผิดให้เปลี่ยนเฉพาะ motor inversion

## Gate 7: Telemetry Watchdog

ถอด TX จาก ESP32 หรือหยุด firmware ชั่วคราว `mobile_robot_hardware` ต้องคืน ERROR หลังเวลาที่กำหนดใน:

```yaml
communication:
  telemetry_timeout_ms: 500
```

## Gate 8: Firmware Fault

ถ้า ESP32 ส่ง `fault_flags != 0` ฝั่ง hardware interface ต้องคืน ERROR หลังแก้ต้นเหตุแล้วจึง clear fault และเริ่ม controller ใหม่ตามขั้นตอนปกติ

## ผ่าน Phase 1B เมื่อ

- firmware build ผ่าน
- ROS 2 build/test ผ่าน
- UART configure ACK ผ่าน
- telemetry ต่อเนื่อง
- ไม่มี fault flags
- joint ซ้าย/ขวาตรงจริง
- motor direction ตรงจริง
- encoder sign ตรงจริง
- timeout ตัดระบบได้จริง
- diff-drive รับคำสั่งความเร็วต่ำและหยุดได้แน่นอน
