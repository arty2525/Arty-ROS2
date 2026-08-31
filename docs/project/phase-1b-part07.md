# Phase 1B Part 07

เพิ่ม UART configuration handshake และ protocol hardening สำหรับ Production:

- เพิ่ม `CommandConfigure (0x06)`
- ESP32 ปฏิเสธค่า encoder counts/revolution ที่เป็นศูนย์ ติดลบ NaN หรือ infinity
- ESP32 ไม่อนุญาต Enable ก่อน Configure
- ACK ผูกกับ sequence ของ command
- Host ตรวจ sequence, command type และ ACK status
- CRC-16/CCITT-FALSE ใช้ทั้ง ESP32 และ ROS 2
- เพิ่ม unit tests ฝั่ง ROS 2

สถานะหลัง boot คือ `configured=false`, `enabled=false`, PWM=0
