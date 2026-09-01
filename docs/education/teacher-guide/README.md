# คู่มือครู — รายวิชาระบบปฏิบัติการหุ่นยนต์ ม.4

กลุ่มสาระการเรียนรู้ **วิทยาศาสตร์และเทคโนโลยี**  
ระยะเวลา **20 สัปดาห์ × 100 นาทีต่อเนื่อง = 2,000 นาที**  
แพลตฟอร์ม **Raspberry Pi 4 + Ubuntu 24.04 LTS + ROS 2 Jazzy + ESP32 + Cytron MDD10A + Encoder Motor**

คู่มือนี้ใช้สำหรับครูผู้สอนเพื่อ **ทบทวนความรู้ก่อนสอน ขยายความเนื้อหา อธิบายเหตุผลของแต่ละขั้นตอน เตรียมการทดลอง และป้องกันความสับสนของนักเรียน** โดยใช้คู่กับบทเรียนสำหรับนักเรียนใน `docs/education/weeks/`.

## วิธีใช้คู่มือครู
ก่อนสอนแต่ละสัปดาห์ให้ใช้วงจร 5 ขั้น:
1. **Review** — ทบทวนแนวคิด/คำศัพท์
2. **Prepare** — เตรียมอุปกรณ์ ซอฟต์แวร์ และระบบสาธิต
3. **Pre-test system** — ครูทดลองด้วยตนเองก่อนสอนอย่างน้อย 1 รอบ
4. **Teach by evidence** — ให้นักเรียนอธิบายจากสิ่งที่สังเกตและวัด
5. **Stop on uncertainty** — หากค่าฮาร์ดแวร์ไม่ทราบ ให้หยุดและวัด ไม่เติมค่าคาดเดา

## โครงสร้างมาตรฐานในแต่ละบท
แต่ละ Week จะมี: ความรู้ที่ครูควรทบทวน, แนวคิดสำคัญ, คำศัพท์, การเตรียมก่อนสอน, ลำดับสอน 100 นาทีแบบละเอียด, จุดที่เด็กมักสับสน, คำถามชี้นำ, ผลที่ควรเห็น, วิธีวิเคราะห์ error, Safety Checkpoint และเนื้อหาขยาย

## คู่มือครูที่จัดทำแล้ว
1. [Week 01 — ระบบหุ่นยนต์และความปลอดภัย](week-01.md)
2. [Week 02 — Ubuntu/Linux](week-02.md)
3. [Week 03 — Git/GitHub](week-03.md)
4. [Week 04 — ROS 2 Jazzy](week-04.md)
5. [Week 05 — ROS 2 Graph](week-05.md)
6. [Week 06 — ROS 2 Workspace/Package/Colcon](week-06.md)
7. [Week 07 — ESP32/PlatformIO](week-07.md)
8. [Week 08 — Motor Driver/PWM/DIR](week-08.md)
9. [Week 09 — Encoder/TPR](week-09.md)
10. [Week 10 — UART/Protocol](week-10.md)

## คู่มือที่จะจัดทำต่อ
11. URDF/Xacro/TF
12. ros2_control
13. Differential Drive/cmd_vel
14. Odometry
15. Diagnostics/Fault Handling
16. Physical Parameter Measurement
17. Calibration/PID
18. System Integration
19. Mini Challenge
20. Final Demonstration/Post-test

## หลักการสำคัญที่ครูต้องย้ำตลอดรายวิชา
### Hardware truth มาก่อน software convenience
ค่าที่มีผลต่อการเคลื่อนที่ เช่น wheel separation, encoder TPR, inversion หรือ PID ต้องมาจากการวัด/ทดลองจริง

### แยก “อาการ” ออกจาก “สาเหตุ”
“หุ่นไม่วิ่ง” เป็นอาการ สาเหตุอาจอยู่ที่ controller, UART, firmware, configuration, power หรือ wiring

### เปลี่ยนทีละหนึ่งตัวแปร
ห้ามเปลี่ยนสาย + parameter + code พร้อมกัน เพราะจะไม่รู้ว่าอะไรทำให้ผลเปลี่ยน

### ต้องมีหลักฐาน
ทุกกิจกรรมควรมี Terminal log, screenshot, measurement table, configuration diff, Git commit SHA, diagnostics หรือ Engineering Logbook อย่างน้อยหนึ่งอย่าง

### Safety มาก่อนความสำเร็จ
การทดลองมอเตอร์ครั้งแรกต้องยกล้อพ้นพื้น จำกัดความเร็ว มีผู้ควบคุมหนึ่งคน มีวิธีตัดไฟ และหยุดเมื่อพฤติกรรมผิดปกติ

> หลักร่วมของทั้งรายวิชา: **Measure → Configure → Test → Record → Improve**
