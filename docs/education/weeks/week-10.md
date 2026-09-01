# สัปดาห์ที่ 10 — UART: Raspberry Pi ↔ ESP32 และ Protocol

**เวลา:** 100 นาที

## จุดประสงค์
นักเรียนต่อ TX/RX ได้ถูกต้อง, อธิบาย baud/frame/CRC/ACK/telemetry/timeout และวิเคราะห์ data flow ระหว่าง ROS 2 กับ firmware ได้.

## Baseline
- Raspberry Pi TX GPIO14 → ESP32 RX2 GPIO16
- Raspberry Pi RX GPIO15 ← ESP32 TX2 GPIO17
- UART 115200 baud

## คาบที่ 1
### Engage — 10 นาที
ครูถาม: ถ้าส่งเพียงตัวเลขมอเตอร์ต่อกันผ่าน serial เราจะรู้ได้อย่างไรว่าข้อมูลเริ่มตรงไหน จบตรงไหน หรือเสียหายระหว่างทาง?

### Explore — 25 นาที
ตรวจสาย **ขณะปิดกำลังที่เกี่ยวข้อง** และวาดเส้น TX→RX / RX←TX. นักเรียนอธิบายเหตุผลที่ TX ไม่ต่อ TX.

สำรวจ source protocol ใน repository โดยไม่แก้ไฟล์ production และระบุองค์ประกอบ frame ที่พบ เช่น magic/version/type/payload/CRC.

### Explain — 15 นาที
อธิบาย UART 115200, frame, CRC16, ACK สำหรับคำสั่ง transactional, telemetry และเหตุผลของ heartbeat/command timeout.

## คาบที่ 2
### Elaborate — 30 นาที
แต่ละกลุ่มสร้าง Sequence Diagram:

`ROS 2 hardware → UART command → ESP32 → motor control`

`Encoder → ESP32 telemetry → UART → ROS 2 hardware → joint state/odometry`

เพิ่มกรณี “การสื่อสารหยุด” และอธิบายว่าทำไม timeout เป็นคุณสมบัติด้าน safety.

### Evaluate — 20 นาที
ชิ้นงาน:
- UART wiring diagram
- protocol frame diagram
- sequence diagram ปกติและกรณี timeout
- Exit Ticket

## คำถามประเมิน
1. Baud rate ของทั้งสองฝั่งต้องสัมพันธ์กันอย่างไร?
2. CRC ช่วยตรวจอะไร?
3. ACK กับ telemetry ต่างกันอย่างไร?
4. Heartbeat timeout ช่วยลดความเสี่ยงอะไร?
5. ทำไม parser ต้องรับมือข้อมูลที่ไม่เริ่มตรงขอบ frame ได้?

## Safety
ทดสอบ UART logic ก่อน Motor Power. การสื่อสารผิดปกติต้องนำระบบเข้าสู่สภาวะปลอดภัย ไม่ควรทำให้มอเตอร์ค้างที่คำสั่งเดิม.
