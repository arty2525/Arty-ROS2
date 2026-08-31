# Phase 1B Part 12

หัวข้อ: UART Streaming Transport Hardening

ปรับ protocol runtime ให้ ACK เฉพาะ transactional commands และยกเลิก ACK สำหรับ `CommandVelocity` และ `Heartbeat`

การเปลี่ยนแปลงนี้แก้ปัญหา ACK สะสมใน RX queue ของ host และทำให้ telemetry มีเส้นทางรับข้อมูลที่คาดการณ์ได้มากขึ้น
