# คู่มือครู Week 02 — Ubuntu/Linux สำหรับระบบหุ่นยนต์

**เวลา:** 100 นาทีต่อเนื่อง  
**บทนักเรียนที่ใช้คู่กัน:** `docs/education/weeks/week-02.md`

## 1. ความรู้ที่ครูควรทบทวนก่อนสอน
Linux เป็นระบบปฏิบัติการหลักของ Raspberry Pi ในโครงการนี้ Ubuntu 24.04 LTS ทำหน้าที่เป็นฐานให้ ROS 2 Jazzy และเครื่องมือพัฒนา ครูควรเข้าใจว่า Terminal เป็นเพียงช่องทางหนึ่งในการสั่งงาน Shell ไม่ใช่ตัวระบบปฏิบัติการทั้งหมด

คำสำคัญที่ควรเข้าใจ:
- **Terminal** หน้าต่างที่ใช้ติดต่อกับ shell
- **Shell** โปรแกรมที่รับคำสั่งและเรียกโปรแกรมอื่น เช่น bash
- **Filesystem** โครงสร้างไฟล์แบบลำดับชั้น เริ่มจาก `/`
- **Home directory** พื้นที่ส่วนตัวของผู้ใช้ เช่น `/home/student`
- **Absolute path** path ที่เริ่มจาก `/`
- **Relative path** path ที่อ้างจาก directory ปัจจุบัน
- **Permission** สิทธิ์ read/write/execute
- **Package manager** เครื่องมือจัดการ software package เช่น `apt`
- **SSH** การเข้า shell ของเครื่องอื่นผ่านเครือข่ายอย่างเข้ารหัส

## 2. แนวคิดที่ครูต้องเน้น
นักเรียนไม่ควรจำคำสั่งเป็นชุดโดยไม่รู้ว่า “ตอนนี้เราอยู่ directory ไหน” และ “คำสั่งนี้จะเปลี่ยนอะไร” ทุกครั้งก่อนคำสั่งที่มีผลต่อไฟล์ควรให้ตรวจ `pwd` และ `ls` ก่อน

`sudo` ไม่ได้แปลว่า “คำสั่งแก้ error” แต่หมายถึงการยกระดับสิทธิ์ชั่วคราว จึงต้องใช้เฉพาะเมื่อเข้าใจว่าคำสั่งต้องการสิทธิ์ระดับ administrator จริง

## 3. เตรียมก่อนเข้าห้อง
1. เตรียม Raspberry Pi/Ubuntu ที่บูตได้อย่างน้อยหนึ่งเครื่อง
2. ตรวจ Internet/เครือข่ายห้องเรียน
3. ตรวจ user/password ของเครื่องสาธิต
4. ติดตั้ง SSH ล่วงหน้าหากต้องสาธิต remote login
5. เตรียม Terminal screenshot ที่มีตัวอย่าง command success และ common error

## 4. อธิบายคำสั่งสำคัญทีละคำสั่ง
### `pwd`
แสดง working directory ปัจจุบัน ใช้ตอบคำถามว่า “ตอนนี้เราอยู่ที่ไหน”

### `ls` และ `ls -la`
`ls` แสดงไฟล์ทั่วไป ส่วน `-l` แสดงรายละเอียด และ `-a` แสดงไฟล์ซ่อนที่ชื่อขึ้นต้นด้วย `.`

### `cd`
เปลี่ยน directory. `cd ~` กลับ home, `cd ..` ขึ้นหนึ่งระดับ

### `mkdir -p`
สร้าง directory. `-p` ช่วยสร้าง parent directory ที่ยังไม่มีและไม่ error เมื่อ directory มีอยู่แล้ว

### `touch`
สร้างไฟล์ว่างหรือปรับ timestamp ของไฟล์ที่มีอยู่

### `cp` / `mv` / `rm`
คัดลอก / ย้ายหรือเปลี่ยนชื่อ / ลบ. ต้องย้ำว่า Linux CLI มักไม่มีถังขยะสำหรับ `rm`

### `cat /etc/os-release`
อ่านข้อมูล distribution/version ของ Ubuntu

### `uname -m`
ดู architecture เช่น `aarch64` บน Raspberry Pi 64-bit

### `ip addr` / `hostname -I`
ดูข้อมูล network interface และ IP address

### `sudo apt update`
ดาวน์โหลดรายการ package รุ่นล่าสุดจาก repository **ยังไม่ใช่การอัปเดตโปรแกรมทั้งหมด**

### `sudo apt install <package>`
ติดตั้ง package และ dependency

## 5. ลำดับสอน 100 นาที
### นาที 0–10 — Engage
แสดง Raspberry Pi ที่ไม่มีจอ แล้ว SSH เข้าเครื่องจาก notebook เพื่อให้เห็นว่าหุ่นยังบริหารจัดการได้โดยไม่ต้องมีจอถาวร

### นาที 10–40 — Explore
ให้นักเรียนทำ Linux Mission ทีละคำสั่ง ครูไม่ควรให้ copy block ยาวทั้งหมดในครั้งเดียว ให้หยุดหลังแต่ละช่วงและถาม “ก่อน–หลังเกิดอะไรขึ้น”

ตัวอย่างลำดับ:
```bash
pwd
ls
mkdir -p ~/robotics_lab/week02
cd ~/robotics_lab/week02
pwd
touch note.txt
ls -la
```
จากนั้นค่อยสอน `cp` และ `mv`

### นาที 40–60 — Explain
วาด filesystem tree บนกระดาน: `/` → `/home` → `/home/student` → `robotics_lab` และอธิบาย absolute/relative path

สาธิต permission ด้วย `ls -l` โดยยังไม่ต้องลงรายละเอียดเลข chmod หากนักเรียนยังใหม่

### นาที 60–85 — Elaborate
สาธิต `apt` และ SSH. ก่อน SSH ให้ใช้ `hostname -I` แล้วชี้ว่า IP คือ address ของเครื่องใน network ไม่ใช่ username

### นาที 85–100 — Evaluate
ให้โจทย์สร้าง directory/ไฟล์โดยไม่ให้คำสั่งสำเร็จรูป เพื่อดูว่านักเรียนเลือกคำสั่งเองได้หรือไม่

## 6. จุดที่นักเรียนมักสับสน
### `apt update` กับ `apt upgrade`
`update` อัปเดตรายการ package; `upgrade` อัปเกรด package ที่ติดตั้งอยู่ตามรายการใหม่

### `/` กับ `~`
`/` คือ root ของ filesystem ส่วน `~` คือ home directory ของ user ปัจจุบัน

### username กับ hostname
username คือบัญชีผู้ใช้ ส่วน hostname คือชื่อเครื่อง

### IP address เปลี่ยนได้
ใน network แบบ DHCP IP อาจเปลี่ยนหลัง reboot จึงไม่ควรจำ IP เป็นข้อมูลตายตัวโดยไม่ตรวจ

## 7. Common errors และวิธีอธิบาย
**`command not found`** — โปรแกรมไม่ติดตั้ง, PATH ไม่มี, หรือสะกดผิด  
**`No such file or directory`** — path ผิดหรือไฟล์ไม่มี  
**`Permission denied`** — สิทธิ์ไม่พอ ไม่ควรแก้ด้วย `sudo` ทันทีจนกว่าจะรู้สาเหตุ  
**SSH timeout/refused** — ตรวจ network, IP, ssh service, firewall/connection ตามลำดับ

## 8. คำถามชี้นำ
- ก่อน `rm` ควรตรวจอะไร? → path และชื่อไฟล์
- ทำไมต้องใช้ `pwd`? → ลดความผิดพลาดจากทำงานผิด directory
- ทำไม ROS robot นิยมใช้ SSH? → เข้าจัดการเครื่อง headless ผ่าน network ได้

## 9. Safety & Data Integrity
แม้เป็น software lesson ก็มีความเสี่ยงต่อข้อมูล ห้ามสอนให้นักเรียนใช้ `sudo rm -rf` เป็นตัวอย่าง และควรแยกโฟลเดอร์ฝึกออกจาก production repository

## 10. เนื้อหาขยาย
อธิบาย process, service, environment variable, PATH และ systemd แบบภาพรวม เพื่อปูพื้นก่อน ROS 2 launch/service
