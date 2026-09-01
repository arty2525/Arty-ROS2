# สัปดาห์ที่ 20 — Final Demonstration, Post-test และสะท้อนผลการเรียนรู้

**เวลา:** 100 นาทีต่อเนื่อง

## เป้าหมาย
นักเรียนสาธิตระบบ Arty-ROS2 อย่างปลอดภัย อธิบาย architecture/data flow แก้ปัญหาจากหลักฐาน และประเมินพัฒนาการของตนเองตลอด 20 สัปดาห์ได้

## กิจกรรมการเรียนรู้ 100 นาทีต่อเนื่อง
### 1. Readiness & Engage — 10 นาที
ทุกทีมส่ง Final Readiness Card: team roles, commit SHA, config status, Safety Checklist, known limitations และ stop procedure. ครูอนุญาตเฉพาะทีมที่ผ่าน safety gate

### 2. Post-test — 20 นาที
แบบประเมินครอบคลุม Linux/Terminal, Git, ROS 2 Graph/Package, ESP32/PlatformIO, PWM/DIR, Encoder/TPR, UART/CRC/timeout, URDF/TF, ros2_control, Differential Drive, Odometry, Diagnostics, Calibration และ Engineering Safety

ใช้ผลเทียบกับ Pre-test เพื่อวัดพัฒนาการ ไม่ใช้คะแนนเดียวตัดสินความสามารถปฏิบัติ

### 3. Final Demonstration — 40 นาที
แต่ละทีมสาธิตตามเวลาที่ครูกำหนด:
1. อธิบาย System Architecture
2. แสดง software/commit baseline
3. แสดง diagnostics ก่อนเคลื่อนที่
4. สาธิต motion mission อย่างปลอดภัย
5. แสดง encoder/odometry evidence
6. อธิบายปัญหาหนึ่งกรณีและวิธี troubleshooting
7. ระบุ limitation/สิ่งที่ยังไม่ได้ calibrate

ทีมอื่นทำ Peer Observation โดยบันทึกสิ่งที่ทำได้ดีและคำถามเชิงเทคนิคหนึ่งข้อ

### 4. Engineering Viva — 15 นาที
ครูสุ่มถามสมาชิกแต่ละคนเพื่อยืนยันความเข้าใจรายบุคคล เช่น “ถ้า UART หยุด ระบบควรทำอะไร?”, “wheel separation ผิดส่งผลอย่างไร?”, “ทำไมต้อง source workspace?”, “diagnostics ช่วยอะไร?”

### 5. Reflection & Evaluate — 15 นาที
นักเรียนเขียน Reflection:
- ก่อนเรียนฉันทำอะไรไม่ได้ แต่ตอนนี้ทำได้อะไร
- หลักฐานชิ้นใดแสดงพัฒนาการมากที่สุด
- failure ที่สำคัญที่สุดและเรียนรู้อะไร
- ถ้ามีเวลาอีก 4 สัปดาห์จะพัฒนาหุ่นส่วนใดและเพราะอะไร

## Final Rubric 100 คะแนน
| ด้าน | คะแนน |
|---|---:|
| ความปลอดภัยและวินัยวิศวกรรม | 20 |
| ความเข้าใจ ROS 2/System Architecture | 20 |
| Hardware/Embedded/Communication | 15 |
| Control/Odometry/Diagnostics | 15 |
| Final Demonstration และความทำซ้ำได้ | 15 |
| Troubleshooting จากหลักฐาน | 10 |
| Documentation/Teamwork | 5 |

## หลักฐานปลายภาค
- Engineering Logbook
- Git commit/config baseline
- Physical Parameter Record
- Encoder/Calibration data
- Integration Checklist
- Mini Challenge Evidence
- Final Demonstration rubric
- Pre/Post assessment
- Individual reflection

## สรุปหลักสูตร
หลัง 20 สัปดาห์ นักเรียนไม่ได้เพียง “สั่งหุ่นให้วิ่ง” แต่ต้องสามารถอธิบายเส้นทางตั้งแต่ `ROS 2 → Controller → Hardware Interface → UART → ESP32 → Motor` และ feedback `Encoder → ESP32 → ROS 2 → Odometry/Diagnostics` พร้อมใช้กระบวนการ **Measure → Configure → Test → Record → Improve** อย่างปลอดภัย
