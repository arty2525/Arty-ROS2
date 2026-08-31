## สรุปการเปลี่ยนแปลง

อธิบายสิ่งที่แก้ไขและเหตุผลที่ต้องแก้ไข

## ประเภทการเปลี่ยนแปลง

- [ ] Firmware ESP32
- [ ] ROS 2 package
- [ ] ros2_control
- [ ] Hardware / Wiring
- [ ] Documentation
- [ ] CI / Tooling
- [ ] Bug fix

## การตรวจสอบ

- [ ] `colcon build` ผ่าน
- [ ] `colcon test` ผ่าน
- [ ] `colcon test-result --verbose` ไม่มี failure
- [ ] `pio run` ผ่าน หากมีการแก้ firmware
- [ ] `mkdocs build --strict --clean` ผ่าน หากมีการแก้เอกสาร
- [ ] ไม่เพิ่มค่าฮาร์ดแวร์ที่คาดเดา
- [ ] ตรวจสอบ fail-safe สำหรับการเปลี่ยนแปลงที่เกี่ยวกับมอเตอร์แล้ว

## Hardware validation

หาก PR นี้มีผลต่อมอเตอร์, encoder, UART หรือไฟฟ้า ให้ระบุผลการทดสอบบนหุ่นยนต์จริง

```text
Hardware tested:
Result:
Safety notes:
```
