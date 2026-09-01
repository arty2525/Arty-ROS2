# สัปดาห์ที่ 7 — ESP32 และ PlatformIO

**เวลา:** 100 นาที

## จุดประสงค์
นักเรียนระบุหน้าที่ ESP32 และ GPIO หลักของโครงการได้, build firmware ด้วย PlatformIO, ตรวจ serial port และ upload firmware อย่างปลอดภัยได้.

## GPIO Baseline ของโครงการ
| หน้าที่ | GPIO |
|---|---:|
| UART RX2 | 16 |
| UART TX2 | 17 |
| PWM LEFT | 25 |
| DIR LEFT | 26 |
| PWM RIGHT | 27 |
| DIR RIGHT | 14 |
| Encoder L A/B | 32 / 33 |
| Encoder R A/B | 34 / 35 |

> GPIO34/35 เป็น input-only และไม่มี internal pull-up. ต้องตรวจชนิด output/ระดับแรงดัน Encoder ก่อนต่อจริง.

## คาบที่ 1
### Engage — 10 นาที
เปรียบเทียบ Raspberry Pi กับ ESP32: ทำไมไม่ให้ Linux สร้าง PWM และอ่าน encoder แบบ real-time ทุกอย่างโดยตรง?

### Explore — 30 นาที
```bash
cd ~/Arty-ROS2/firmware/esp32
cat platformio.ini
pio --version
pio run
```
นักเรียนระบุ board/framework/build flags จากไฟล์จริง.

### Explain — 10 นาที
อธิบาย compile → link → firmware image → upload และหน้าที่ของ microcontroller ใน control loop.

## คาบที่ 2
### Explore — 20 นาที
ต่อ ESP32 ด้วย USB โดย **ยังไม่ต่อกำลังมอเตอร์**:
```bash
pio device list
ls /dev/ttyUSB* /dev/ttyACM* 2>/dev/null
```

### Elaborate — 15 นาที
เมื่อครูตรวจการต่อแล้ว:
```bash
cd ~/Arty-ROS2/firmware/esp32
pio run -t upload
```
บันทึกผล upload และ port ที่ใช้.

### Evaluate — 15 นาที
นักเรียนส่ง build/upload log, pin map และอธิบายหน้าที่ RX/TX/PWM/DIR/Encoder.

## Safety Checkpoint
- ไม่ต่อ Motor Power ในกิจกรรม build/upload
- ตรวจ USB/serial device ก่อน upload
- ห้ามย้าย GPIO จาก production baseline โดยไม่มีเหตุผลและหลักฐาน
- ห้ามต่อสัญญาณที่แรงดันเกินระดับที่ ESP32 รับได้
