# คู่มือครู Week 07 — ESP32 และ PlatformIO

**เวลา:** 100 นาทีต่อเนื่อง  
**ใช้คู่กับ:** `docs/education/weeks/week-07.md`

## ความรู้ที่ครูควรทบทวน
ESP32 ในโครงการนี้ทำหน้าที่ควบคุมระดับ Embedded รับคำสั่งจาก Raspberry Pi ผ่าน UART สร้างสัญญาณ PWM/DIR ให้ Motor Driver อ่าน Encoder และส่ง telemetry กลับ

GPIO baseline:
- RX2 GPIO16
- TX2 GPIO17
- PWM_LEFT GPIO25
- DIR_LEFT GPIO26
- PWM_RIGHT GPIO27
- DIR_RIGHT GPIO14
- Encoder Left A/B GPIO32/33
- Encoder Right A/B GPIO34/35

GPIO34/35 เป็น input-only และไม่มี internal pull-up จึงต้องตรวจชนิด output ของ Encoder และวงจร pull-up ภายนอกหากอุปกรณ์ต้องใช้

PlatformIO เป็นเครื่องมือจัดการ environment, framework, library, build และ upload firmware ไม่ใช่ภาษาโปรแกรม

## เตรียมก่อนสอน
1. ตรวจ USB cable ว่าส่งข้อมูลได้ ไม่ใช่สายชาร์จอย่างเดียว
2. ตรวจว่า PlatformIO ใช้งานได้ด้วย `pio --version`
3. ทดลอง `pio run` ใน `firmware/esp32`
4. ตรวจ port ของ ESP32
5. ยังไม่ต่อ Motor Power ระหว่าง build/upload
6. เตรียมบอร์ดสำรองอย่างน้อยหนึ่งตัวถ้าเป็นไปได้

## ขั้นตอนสอน 100 นาที
### นาที 0–10: Engage
ถามว่า “ทำไมไม่ให้ Raspberry Pi ขับ PWM และอ่าน Encoder ทุกอย่างเอง?” พาเด็กไปสู่แนวคิดว่า Linux เป็น general-purpose OS ส่วนงาน embedded/control loop มักต้องการ timing ที่คงที่กว่า

### นาที 10–40: Explore
```bash
cd ~/Arty-ROS2/firmware/esp32
cat platformio.ini
pio --version
pio run
```
ให้นักเรียนหาว่า project ใช้ board, platform และ framework อะไรจากไฟล์จริง ไม่ให้ครูบอกก่อน

### นาที 40–60: Explain
อธิบาย build pipeline: source code → preprocess/compile → object files → link → firmware image → upload

อธิบายว่า compile success หมายถึง code ผ่านขั้น build ไม่ได้แปลว่า wiring/hardware ถูกต้อง

### นาที 60–85: Elaborate
ต่อ USB และใช้:
```bash
pio device list
ls /dev/ttyUSB* /dev/ttyACM* 2>/dev/null
```
จากนั้นเมื่อครูตรวจแล้ว:
```bash
pio run -t upload
```
ให้นักเรียนจด port, board และผล upload ลง Logbook

### นาที 85–100: Evaluate
ถามความหมายของ RX/TX/PWM/DIR/Encoder และให้เด็กอธิบายว่า upload firmware ต่างจาก build อย่างไร

## จุดสับสนที่พบบ่อย
**Build ผ่าน = firmware อยู่ในบอร์ดแล้ว?** ไม่ใช่ ต้อง upload จึงเขียน firmware ลงบอร์ด

**Serial port = GPIO UART เดียวกับ GPIO16/17 หรือไม่?** USB serial ที่เห็นเป็น device บน Linux อาจผ่าน USB-to-UART/USB interface ของบอร์ด ขณะที่ GPIO16/17 เป็น UART2 ที่ใช้สื่อสารกับ Raspberry Pi ตามโครงการ ต้องแยกบริบท

**GPIO34/35 ใช้ OUTPUT ได้หรือไม่?** ไม่ได้ เป็น input-only

## Common errors
- `pio: command not found` → PATH/installation
- ไม่พบ serial port → cable, driver/interface, board power, permission
- permission denied → ตรวจกลุ่ม `dialout` และ login session
- upload timeout → port ผิด, board state หรือการเชื่อมต่อ

## Safety
กิจกรรม firmware ไม่ต้องใช้ Motor Power. ถ้ามีการต่อวงจรบนหุ่น ให้ตัดแหล่งกำลังมอเตอร์ก่อนทุกครั้ง

## เนื้อหาขยาย
อธิบาย watchdog, deterministic loop และเหตุผลที่ firmware มี command/heartbeat timeout เพื่อไม่ให้มอเตอร์ค้างเมื่อ host หาย
