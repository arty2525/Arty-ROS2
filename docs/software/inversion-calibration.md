# Motor และ Encoder Inversion

Phase1B-Part13 ยกเลิกการ hardcode ทิศทางมอเตอร์และ encoder ใน firmware และขยาย `CommandConfigure` ให้รองรับ inversion แยกซ้าย/ขวา

ค่าที่ protocol รองรับ:

- `left_motor_inverted`
- `right_motor_inverted`
- `left_encoder_inverted`
- `right_encoder_inverted`

ESP32 จะหยุดมอเตอร์และ reset encoder ก่อนใช้ inversion ใหม่

## Calibration

1. ยกล้อให้พ้นพื้น
2. เริ่มจาก inversion ทุกตัวเป็น false
3. สั่งความเร็วบวกทีละล้อด้วยค่าต่ำ
4. ถ้าล้อหมุนกลับทิศ ให้กลับ motor inversion ของล้อนั้น
5. เมื่อหมุนไปข้างหน้า encoder ต้องเพิ่มค่า
6. ถ้า encoder ลด ให้กลับ encoder inversion ของล้อนั้น

Part ถัดไปจะเชื่อมค่าทั้ง 4 จาก ROS 2 hardware parameters ไปยัง ConfigurePayload โดยตรง
