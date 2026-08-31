# Phase1B-Part25 — Physical Parameter Verification Integrity

Part นี้แก้ความไม่สอดคล้องของ verification เดิมที่ฝังค่า TPR และมิติฐานหุ่นยนต์ซึ่งยังไม่ผ่านการวัดจริงไว้ใน `verify_phase1b_part16.sh`

การเปลี่ยนแปลงหลัก:

- ลบค่ากายภาพสมมติออกจาก Phase1B-Part16 verifier
- ให้ verifier อ่านค่าจาก production `robot.yaml` โดยตรง
- เพิ่ม `validate_robot_config.py` สำหรับตรวจ schema เชิงปฏิบัติ ชนิดข้อมูล ค่า finite และค่าติดลบ
- ล็อกรัศมีล้อที่ยืนยันแล้วไว้ที่ `0.0325 m`
- รองรับ `--require-complete` เพื่อบังคับไม่ให้มีค่า `0.0` ก่อนนำระบบไปใช้งานจริง
- verifier ไม่มีการฝังค่า TPR หรือมิติฐานสมมติ และใช้ค่าจาก production config เท่านั้น

Part นี้ไม่เติมค่าที่ยังไม่ทราบ ได้แก่ TPR, wheel separation, wheel width, base dimensions, caster geometry และ motion limits
