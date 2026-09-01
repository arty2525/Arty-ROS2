# สัปดาห์ที่ 15 — Diagnostics, Fault Handling และ Fail-safe

**เวลา:** 100 นาทีต่อเนื่อง

## จุดประสงค์
นักเรียนอ่าน diagnostics, อธิบาย fault flags หลัก, แยก warning/error และสร้างขั้นตอน troubleshooting/fault recovery ที่ปลอดภัยได้

## Fault ที่โครงการตรวจได้
- Command timeout
- Heartbeat timeout
- Invalid configuration
- Protocol error
- Invalid velocity command
- Unknown fault bits (หากมี)

## กิจกรรมการเรียนรู้ 100 นาทีต่อเนื่อง
### 1. Engage — 10 นาที
สถานการณ์: หุ่นกำลังเคลื่อนที่แล้วสาย UART หลุด. ถ้า firmware ไม่มี timeout จะเกิดความเสี่ยงอะไร?

### 2. Explore — 25 นาที
ตรวจ topic:
```bash
source /opt/ros/jazzy/setup.bash
ros2 topic list -t | grep diagnostic
```
หากระบบกำลังทำงาน:
```bash
ros2 topic echo /diagnostics
```
นักเรียนค้น status level, message, key/value และ fault information. หากหุ่นยังไม่พร้อม ใช้ diagnostic log ที่ครูเตรียม.

### 3. Explain — 20 นาที
อธิบาย health monitoring หลายชั้น: ROS diagnostics → hardware interface → UART health → firmware fault → motor safe state. แยก “อาการ” เช่นไม่มี odom ออกจาก “สาเหตุ” เช่น serial timeout.

### 4. Elaborate — 30 นาที
ทำ Fault Investigation แบบปลอดภัยโดย **ไม่สร้าง electrical short และไม่ดึงสายขณะมอเตอร์กำลังขับ**. ใช้ simulation/log หรือวิธี software ที่ครูกำหนดเพื่อศึกษากรณี timeout/invalid configuration.

แต่ละกลุ่มกรอก:
| อาการ | Diagnostic/Fault | สมมติฐาน | การตรวจยืนยัน | วิธีคืนระบบ |
|---|---|---|---|---|

สร้าง troubleshooting flow: Stop → Read diagnostics → Record evidence → Identify layer → Correct one cause → Re-test → Confirm fault cleared.

### 5. Evaluate — 15 นาที
ส่ง fault worksheet + troubleshooting flowchart + อธิบาย fail-safe ด้วยภาษาตนเอง + Exit Ticket.

## Safety Rule
Fault test ต้องทำให้ระบบปลอดภัยกว่าเดิม ไม่ใช่สร้างความเสียหายเพื่อให้เกิด error. ห้าม short circuit, reverse polarity หรือบังคับมอเตอร์ติดขัดเพื่อทดสอบ fault.

## เกณฑ์
จำแนก fault ≥80% • troubleshooting เป็นลำดับและมีหลักฐาน • recovery ไม่ bypass safety • Teamwork ระดับ 3/4 ขึ้นไป.
