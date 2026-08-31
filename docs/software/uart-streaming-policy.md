# UART Streaming Policy

คำสั่ง UART ถูกแบ่งเป็น 2 กลุ่ม

## Transactional commands

คำสั่งต่อไปนี้ต้องได้รับ ACK เพราะ host ต้องทราบผลก่อนเปลี่ยน state:

- `CommandConfigure`
- `CommandEnable`
- `CommandSetPid`
- `CommandClearFaults`

ACK ต้องมี sequence เดียวกับคำสั่งต้นทาง

## Streaming commands

คำสั่งต่อไปนี้ **ไม่มี ACK**:

- `CommandVelocity`
- `Heartbeat`

เหตุผลคือทั้งสองคำสั่งถูกส่งซ้ำเป็นระยะ การ ACK ทุก packet ทำให้ RX queue ฝั่ง Raspberry Pi สะสม ACK ที่ host ไม่ได้รอ และเพิ่มโอกาสให้ telemetry ถูกหน่วง

ความถูกต้องของ streaming command ตรวจสอบผ่าน:

- command timeout
- heartbeat timeout
- telemetry
- fault flags

หาก streaming payload ไม่ถูกต้อง ESP32 จะตั้ง fault flag และหยุดมอเตอร์เมื่อเป็นกรณีที่กระทบความปลอดภัย โดยไม่สร้าง ACK เพิ่มเข้าคิว
