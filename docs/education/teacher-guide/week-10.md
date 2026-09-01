# คู่มือครู Week 10 — UART และ Protocol ระหว่าง Raspberry Pi กับ ESP32

**เวลา:** 100 นาทีต่อเนื่อง  
**ใช้คู่กับ:** `docs/education/weeks/week-10.md`

## ความรู้ที่ครูควรทบทวน
UART เป็นการสื่อสารอนุกรมแบบ asynchronous ที่กำหนด baud rate ร่วมกันระหว่างสองฝั่ง ในโครงการนี้ Raspberry Pi และ ESP32 สื่อสารที่ 115200 baud

Baseline wiring:
- Raspberry Pi TX GPIO14 → ESP32 RX2 GPIO16
- Raspberry Pi RX GPIO15 ← ESP32 TX2 GPIO17

คำสำคัญที่ครูควรเข้าใจ:
- TX/RX: ส่ง/รับข้อมูล
- Baud rate: อัตราการส่งสัญลักษณ์ต่อวินาทีในบริบท UART
- Frame: โครงสร้างข้อมูลที่โปรแกรมกำหนดสำหรับห่อ message
- Payload: ข้อมูลภายใน frame
- CRC: checksum สำหรับตรวจความผิดพลาดของข้อมูล
- ACK: การตอบรับคำสั่งชนิด transaction
- Telemetry: ข้อมูลสถานะที่ส่งกลับต่อเนื่อง
- Heartbeat: สัญญาณบอกว่าฝั่งสื่อสารยังมีชีวิต
- Timeout: เงื่อนไขเมื่อไม่รับข้อมูลภายในเวลาที่กำหนด

## แนวคิดที่ต้องเน้น
UART ระดับไฟฟ้าเพียงทำให้ byte เดินทางได้ แต่ระบบหุ่นยนต์ยังต้องมี **protocol** เพื่อรู้ว่า byte ชุดใดเป็นคำสั่งอะไร เริ่ม/จบตรงไหน และข้อมูลเสียหายหรือไม่

เปรียบเทียบกับจดหมาย: UART = ระบบขนส่งซอง ส่วน protocol = รูปแบบที่อยู่/ประเภทเอกสาร/วิธีตรวจความถูกต้องในซอง

## การเตรียมก่อนสอน
1. ตรวจสาย TX→RX และ RX←TX
2. ตรวจ common reference/ground ตามวงจร
3. ทดสอบ host และ firmware ด้วย build ล่าสุด
4. เตรียม protocol header/source ให้เด็กดูแบบ read-only
5. เตรียม sequence diagram เปล่า
6. ยังไม่ต้องใช้ Motor Power ในช่วงวิเคราะห์ protocol

## ขั้นตอนสอน 100 นาที
### นาที 0–10: Engage
เขียน byte ต่อกันบนกระดาน เช่น `12 34 56 78 ...` แล้วถามว่า “เราจะรู้ได้อย่างไรว่าคำสั่งหนึ่งเริ่มและจบตรงไหน?”

### นาที 10–35: Explore
ให้นักเรียนตรวจ wiring และวาดลูกศร TX→RX. จากนั้นเปิด source protocol และหา field สำคัญ เช่น magic, version, type, payload length และ CRC ตาม implementation จริงของ repository

ครูไม่ควรให้เด็กแก้ source protocol ในสัปดาห์นี้ จุดประสงค์คืออ่านและเข้าใจ contract ก่อน

### นาที 35–55: Explain
อธิบายการสื่อสารเป็นชั้น:
`ROS command → host frame → UART bytes → firmware parser → command execution`
และขากลับ:
`encoder/status → telemetry frame → UART → host parser → ROS state`

อธิบาย CRC ว่าเป็นการตรวจความผิดพลาด ไม่ใช่การเข้ารหัสความลับ

### นาที 55–85: Elaborate
ให้นักเรียนสร้าง Sequence Diagram 2 กรณี:
1. ปกติ: configure/command → ESP32 → telemetry/ACK
2. Communication loss: heartbeat หาย → timeout → safe state/fault

ชี้ให้เห็นว่า streaming command บางชนิดไม่ควรสร้าง ACK ทุกครั้งหากจะทำให้ channel หนาแน่นเกินไป ขณะที่คำสั่ง transactional ต้องมีผลตอบรับที่ตรวจได้

### นาที 85–100: Evaluate
ถาม:
- ทำไม TX ต้องต่อ RX?
- baud rate ต่างกันเกิดอะไร?
- CRC มีหน้าที่อะไร?
- ACK ต่างจาก telemetry อย่างไร?
- timeout สำคัญต่อ safety อย่างไร?

## จุดสับสนที่พบบ่อย
**CRC = Encryption?** ไม่ใช่ CRC ตรวจ corruption ไม่ได้ปกปิดข้อมูล

**ACK ทุก packet ดีที่สุด?** ไม่เสมอ ต้องออกแบบตามชนิดข้อมูลและ bandwidth/latency

**UART ทำงาน = protocol ถูกต้อง?** ไม่เสมอ Byte อาจวิ่งได้แต่ parser/type/payload/CRC อาจไม่ตรงกัน

**TX ต่อ TX ได้ไหม?** โดยทั่วไปในการเชื่อม UART point-to-point แบบนี้ต้อง cross TX→RX และ RX←TX

## Common problems
- ไม่มีข้อมูล → ตรวจ wiring, port/UART selection, baud, ground, firmware state
- อ่านเป็นข้อมูลมั่ว → ตรวจ baud/frame interpretation/protocol mismatch
- frame หลุด sync → parser ต้องค้นหา magic/start ใหม่อย่างปลอดภัย
- communication หายแล้ว motor ยังทำงานต่อ → ถือเป็น safety defect ต้องหยุดทดสอบและตรวจ timeout lifecycle

## วิธีแก้ปัญหาเป็นลำดับ
1. ตรวจ power/ground
2. ตรวจ TX/RX
3. ตรวจ baud
4. ตรวจว่าทั้งสองฝั่งใช้ protocol version เดียวกัน
5. ตรวจ frame type/payload length
6. ตรวจ CRC
7. ตรวจ timeout/heartbeat behavior

## เนื้อหาขยาย
อธิบายว่าการออกแบบ protocol ที่ดีต้องคำนึงถึง framing, validation, backward compatibility, malformed input และ resynchronization หลัง byte สูญหาย
