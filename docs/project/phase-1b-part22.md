# Phase 1B Part 22

หัวข้อ: GitHub Actions CI Repair

ตรวจสอบ GitHub Actions run ของ Part21 แล้วพบความล้มเหลวจริงที่ขั้นตอน `actions/setup-python`

สาเหตุ:

`cache: pip` ถูกเปิดใช้งาน แต่ ESP32 job ไม่มี `requirements.txt` หรือ `pyproject.toml` ที่ setup-python ใช้เป็น cache dependency file

ข้อความจาก GitHub Actions:

`No file ... matched to [**/requirements.txt or **/pyproject.toml]`

Documentation job ล้มเหลวที่ setup-python ด้วยเหตุผลเดียวกัน แม้ repository จะมี `requirements-docs.txt` เพราะ workflow ไม่ได้ระบุ `cache-dependency-path`

การแก้ไข:

- เพิ่ม `requirements-platformio.txt`
- pin `platformio==6.1.18`
- ESP32 job ใช้ `cache-dependency-path: requirements-platformio.txt`
- Documentation job ใช้ `cache-dependency-path: requirements-docs.txt`
- เปลี่ยนคำสั่งติดตั้ง PlatformIO ให้ติดตั้งจาก requirements file
- ไม่เปลี่ยน ROS 2 build/test behavior

Part นี้แก้จาก log ของ CI จริง ไม่ได้คาดเดาสาเหตุ
