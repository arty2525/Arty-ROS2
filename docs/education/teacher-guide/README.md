# คู่มือครู — รายวิชาระบบปฏิบัติการหุ่นยนต์ ม.4

กลุ่มสาระการเรียนรู้ **วิทยาศาสตร์และเทคโนโลยี**  
ระยะเวลา **20 สัปดาห์ × 100 นาทีต่อเนื่อง = 2,000 นาที**  
แพลตฟอร์ม **Raspberry Pi 4 + Ubuntu 24.04 LTS + ROS 2 Jazzy + ESP32 + Cytron MDD10A + Encoder Motor**

คู่มือนี้ใช้สำหรับครูผู้สอนเพื่อ **ทบทวนความรู้ก่อนสอน ขยายความเนื้อหา อธิบายเหตุผลของแต่ละขั้นตอน เตรียมการทดลอง และป้องกันความสับสนของนักเรียน** โดยใช้คู่กับบทเรียนใน `docs/education/weeks/`.

## วิธีใช้คู่มือครู
ก่อนสอนแต่ละสัปดาห์ใช้วงจร 5 ขั้น: **Review → Prepare → Pre-test → Teach by evidence → Stop on uncertainty**. ครูควรทดลองระบบด้วยตนเองก่อนสอน และไม่ใช้ค่าฮาร์ดแวร์ที่คาดเดา

## คู่มือครูครบ 20 สัปดาห์
1. [Week 01 — ระบบหุ่นยนต์และความปลอดภัย](week-01.md)
2. [Week 02 — Ubuntu/Linux](week-02.md)
3. [Week 03 — Git/GitHub](week-03.md)
4. [Week 04 — ROS 2 Jazzy](week-04.md)
5. [Week 05 — ROS 2 Graph](week-05.md)
6. [Week 06 — Workspace/Package/Colcon](week-06.md)
7. [Week 07 — ESP32/PlatformIO](week-07.md)
8. [Week 08 — Motor Driver/PWM/DIR](week-08.md)
9. [Week 09 — Encoder/TPR](week-09.md)
10. [Week 10 — UART/Protocol](week-10.md)
11. [Week 11 — URDF/Xacro/TF](week-11.md)
12. [Week 12 — ros2_control](week-12.md)
13. [Week 13 — Differential Drive/cmd_vel](week-13.md)
14. [Week 14 — Odometry](week-14.md)
15. [Week 15 — Diagnostics/Fault Handling](week-15.md)
16. [Week 16 — Physical Parameter Measurement](week-16.md)
17. [Week 17 — Calibration/PID](week-17.md)
18. [Week 18 — System Integration](week-18.md)
19. [Week 19 — Mini Challenge](week-19.md)
20. [Week 20 — Final Demonstration/Post-test](week-20.md)

## โครงสร้างมาตรฐาน
แต่ละบทมีความรู้ที่ครูควรทบทวน, คำศัพท์, เหตุผลเชิงเทคนิค, การเตรียมก่อนสอน, ลำดับ 100 นาที, จุดที่นักเรียนมักสับสน, คำถามชี้นำ, ผลที่ควรเห็น, troubleshooting, Safety และเนื้อหาขยาย

## หลักการร่วม
- **Hardware truth มาก่อน software convenience** — geometry, TPR, inversion, calibration/PID ต้องอ้างอิงการวัดหรือการทดลองจริง
- **แยกอาการออกจากสาเหตุ** — “หุ่นไม่วิ่ง” เป็น symptom ไม่ใช่ root cause
- **เปลี่ยนทีละหนึ่งตัวแปร** — เพื่อระบุเหตุและผลได้
- **ต้องมีหลักฐาน** — terminal log, measurement, diagnostic, configuration หรือ Git SHA
- **Safety มาก่อนความสำเร็จ** — test แรกยกล้อ, จำกัดความเร็ว, one operator, stop path

> กระบวนการหลักของรายวิชา: **Measure → Configure → Test → Record → Improve**
