# ACK Status Contract

Phase1B-Part27 ล็อกสถานะ ACK ที่สำเร็จให้มีค่า wire เท่ากับ `0`

ชื่อหลักของ protocol คือ `AckStatus::kAccepted` และเพิ่ม `AckStatus::kOk` เป็น compatibility alias ที่มีค่าเดียวกัน เพื่อให้ hardware transport ที่มีอยู่ยัง compile ได้โดยไม่เปลี่ยน wire protocol หรือ firmware behavior

Compatibility alias นี้เป็นการรักษาความเข้ากันได้ของ source code เท่านั้น ไม่ได้สร้าง ACK status ใหม่บนสาย UART

พร้อมกันนี้ protocol test สำหรับ `ConfigurePayload` ถูกแก้ไม่ให้ใช้ `2048.0F` ซึ่งมีลักษณะเหมือนค่า encoder calibration จริง โดยเปลี่ยนเป็น deterministic serialization sentinel ที่ตรวจเทียบด้วย bit pattern
