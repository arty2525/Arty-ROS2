# Phase 1B Part 08

หัวข้อ: Production CI และ GitHub Quality Gate

เพิ่ม GitHub Actions สำหรับ ROS 2 Jazzy build/test, ESP32 PlatformIO build และ MkDocs strict build รวมถึง Dependabot, Pull Request template, structured bug report และ local CI script

เป้าหมายคือให้ทุกการเปลี่ยนแปลงใน `main` ตรวจสอบ build และ test ซ้ำได้อัตโนมัติ โดย CI ไม่ใช้แทนการทดสอบฮาร์ดแวร์จริง
