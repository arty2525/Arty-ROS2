# สัปดาห์ที่ 7 — ESP32 และ PlatformIO

**เวลา:** 100 นาทีต่อเนื่อง

## จุดประสงค์
ระบุหน้าที่ ESP32/GPIO, build firmware, ตรวจ serial port และ upload อย่างปลอดภัยได้

## GPIO Baseline
| หน้าที่ | GPIO |
|---|---:|
| UART RX2/TX2 | 16 / 17 |
| PWM LEFT/DIR LEFT | 25 / 26 |
| PWM RIGHT/DIR RIGHT | 27 / 14 |
| Encoder L A/B | 32 / 33 |
| Encoder R A/B | 34 / 35 |

> GPIO34/35 input-only และไม่มี internal pull-up; ต้องตรวจ output/แรงดัน Encoder ก่อนต่อจริง.

## กิจกรรมการเรียนรู้ 100 นาทีต่อเนื่อง
### 1. Engage — 10 นาที
เปรียบเทียบ Raspberry Pi กับ ESP32: ทำไม Linux ไม่ควรรับภาระ real-time motor/encoder ทุกอย่างโดยตรง?

### 2. Explore — 30 นาที
```bash
cd ~/Arty-ROS2/firmware/esp32
cat platformio.ini
pio --version
pio run
```
ระบุ board/framework/build flags จากไฟล์จริง.

### 3. Explain — 20 นาที
อธิบาย compile → link → firmware image → upload และบทบาท microcontroller ใน control loop.

### 4. Elaborate — 25 นาที
ต่อ ESP32 ด้วย USB โดยยังไม่ต่อ Motor Power:
```bash
pio device list
ls /dev/ttyUSB* /dev/ttyACM* 2>/dev/null
cd ~/Arty-ROS2/firmware/esp32
pio run -t upload
```
ทำ upload หลังครูตรวจการต่อและบันทึก port/result.

### 5. Evaluate — 15 นาที
ส่ง build/upload log + pin map + อธิบาย RX/TX/PWM/DIR/Encoder.

## Safety
ไม่ต่อ Motor Power ระหว่างกิจกรรม firmware • ตรวจ serial device • ไม่ย้าย GPIO โดยไม่มีหลักฐาน • ห้ามป้อนแรงดันเกินระดับ ESP32.
