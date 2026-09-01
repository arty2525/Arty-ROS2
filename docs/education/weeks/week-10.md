# สัปดาห์ที่ 10 — UART: Raspberry Pi ↔ ESP32 และ Protocol

**เวลา:** 100 นาทีต่อเนื่อง

## จุดประสงค์
ต่อ TX/RX ถูกต้อง, อธิบาย baud/frame/CRC/ACK/telemetry/timeout และวิเคราะห์ data flow ระหว่าง ROS 2 กับ firmware ได้

## Baseline
Raspberry Pi TX GPIO14 → ESP32 RX2 GPIO16 • Raspberry Pi RX GPIO15 ← ESP32 TX2 GPIO17 • UART 115200 baud

## กิจกรรมการเรียนรู้ 100 นาทีต่อเนื่อง
### 1. Engage — 10 นาที
ถาม: หากส่งตัวเลขต่อกันผ่าน serial เราจะรู้ได้อย่างไรว่าข้อมูลเริ่ม/จบตรงไหนหรือเสียหายระหว่างทาง?

### 2. Explore — 25 นาที
ขณะปิดกำลังที่เกี่ยวข้อง ตรวจสายและวาด TX→RX / RX←TX. สำรวจ protocol source โดยไม่แก้ production file และหา magic/version/type/payload/CRC.

### 3. Explain — 20 นาที
อธิบาย UART 115200, framing, CRC16, transactional ACK, telemetry และ heartbeat/command timeout.

### 4. Elaborate — 30 นาที
สร้าง Sequence Diagram:
`ROS 2 hardware → UART command → ESP32 → motor control`
และ `Encoder → ESP32 telemetry → UART → ROS 2 hardware → joint state/odometry`.
เพิ่มกรณี communication loss และอธิบาย timeout ในฐานะ safety mechanism.

### 5. Evaluate — 15 นาที
ส่ง UART wiring + protocol frame + sequence diagram ปกติ/timeout + Exit Ticket. ตอบ baud relation, CRC, ACK vs telemetry, heartbeat timeout และ parser resynchronization.

## Safety
ทดสอบ UART logic ก่อน Motor Power. Communication fault ต้องนำระบบสู่สภาวะปลอดภัย ไม่ปล่อยมอเตอร์ค้างที่คำสั่งเดิม.
