# Continuous Integration

Repository ใช้ GitHub Actions ตรวจสอบทุก push และ pull request ที่เข้า `main`

## ROS 2 Jazzy

ตรวจสอบด้วย `rosdep`, `colcon build`, `colcon test` และ `colcon test-result --verbose` บน Ubuntu 24.04

## ESP32

Job สำหรับ ESP32 ติดตั้ง PlatformIO และรัน `pio run` ที่ `firmware/esp32`

การ build ผ่าน CI ยืนยันเพียงว่า firmware compile ได้ ไม่ถือเป็นการยืนยันการทำงานของมอเตอร์บนฮาร์ดแวร์จริง

## Documentation

เอกสารถูก build ด้วย `mkdocs build --strict --clean` และ warning ที่ทำให้ strict mode ล้มเหลวต้องแก้ก่อน merge

## Local CI

ก่อน push สามารถรัน:

```bash
./scripts/ci_local.sh
```

การแก้ PWM/DIR, Encoder, UART, motor enable, watchdog, controller limits, geometry หรือ ticks per revolution ต้องมีการทดสอบบนหุ่นยนต์จริงก่อนถือว่า validated
