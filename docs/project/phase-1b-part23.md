# Phase 1B Part 23

หัวข้อ: CI Build Failure Repair

ตรวจสอบ GitHub Actions run `33390101100` จาก log จริงแล้วพบ 2 สาเหตุใหม่

## ESP32 PlatformIO

PlatformIO ใช้ Arduino ESP32 toolchain ที่ใส่ `-std=gnu++11` มาโดยค่าเริ่มต้น

แม้ `platformio.ini` จะมี `-std=gnu++17` แต่ flag เดิมยังคงอยู่ และ compiler ใช้มาตรฐานที่ทำให้:

- `std::clamp` ใช้งานไม่ได้
- digit separator เช่น `1'000'000.0F` parse ไม่ได้
- nested namespace syntax ถูกเตือนว่า C++17 เท่านั้น

แก้โดยเพิ่ม:

```ini
build_unflags =
    -std=gnu++11
```

และคง:

```ini
build_flags =
    -std=gnu++17
```

## Documentation

Documentation job ติดตั้ง dependencies สำเร็จแล้ว แต่ `mkdocs build --strict --clean` ล้มเหลวเพราะ root repository ไม่มี `mkdocs.yml`

แก้โดยสร้าง root `mkdocs.yml` ที่ชี้ `docs_dir: docs` และใช้ข้อมูล repository `arty2525/Arty-ROS2`

Part นี้แก้จาก GitHub Actions log จริงและไม่เปลี่ยน motor-control behavior
