# Host UART Recovery

Phase1B-Part18 ปรับ UART ฝั่ง Raspberry Pi ให้ใช้ stream parser แบบ byte-by-byte เช่นเดียวกับ ESP32

## พฤติกรรม

ฝั่ง host จะไม่ใช้ `tcflush(TCIFLUSH)` เมื่อพบ header ผิดอีกต่อไป เพราะการ flush สามารถทิ้ง telemetry หรือ ACK ที่ถูกต้องซึ่งตามหลัง frame เสียมาแล้วใน UART buffer

ตัว parser จะ:

- ค้นหา magic `0xA55A`
- รองรับข้อมูลที่อ่านมาเป็นชิ้นเล็กหรือหลาย frame ใน read เดียว
- รองรับ noise ก่อน frame
- ทิ้ง frame ที่ CRC ผิด
- resync แล้วรับ frame ที่ถูกต้องถัดไป
- เก็บ frame ส่วนเกินไว้ใน queue เพื่อให้ consumer อ่านในรอบถัดไป

## Fault recovery ตอน configure

หลัง `Configure` ได้ ACK แล้ว host จะส่ง `ClearFaults` แบบ transactional และรอ ACK ก่อนถือว่า hardware configure สำเร็จ

จุดประสงค์คือเคลียร์ fault เก่าที่ค้างจาก session ก่อนหน้า เช่น protocol error หรือ timeout ที่เกิดก่อน host เชื่อมต่อใหม่

หาก firmware ไม่ตอบรับ `ClearFaults` การ configure จะล้มเหลวและ serial จะถูกปิดทันที

## Safety

การ clear fault ไม่ได้ใช้เพื่อข้าม fault ที่ยังเกิดอยู่

หากเงื่อนไข fault ยังมีอยู่ firmware จะสามารถตั้ง fault ใหม่ในรอบ control ถัดไป และ ROS 2 hardware interface จะคืน ERROR จาก telemetry ตามเดิม
