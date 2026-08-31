# Phase1B-Part27 — ACK Status Contract Integrity

แก้ความไม่ตรงกันของชื่อ success status ระหว่าง hardware source และ protocol header โดยรักษาค่า wire เดิมไว้ที่ `0`

เพิ่ม unit test ตรวจว่า:

- `kAccepted == 0`
- `kOk == kAccepted`

และลบค่าทดสอบ `2048.0F` ออกจาก Configure round-trip test เพื่อไม่ให้ค่าทดสอบถูกเข้าใจว่าเป็น TPR ที่ยืนยันแล้ว
