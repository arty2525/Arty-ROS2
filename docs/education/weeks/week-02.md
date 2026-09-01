# สัปดาห์ที่ 2 — Ubuntu/Linux สำหรับระบบหุ่นยนต์

**เวลา:** 100 นาทีต่อเนื่อง

## เป้าหมาย
ใช้ Terminal จัดการไฟล์ ตรวจระบบ ติดตั้ง package และเข้าใจ SSH เบื้องต้นได้

## กิจกรรมการเรียนรู้ 100 นาทีต่อเนื่อง
### 1. Engage — 10 นาที
ถามว่าเหตุใดหุ่นยนต์ที่ไม่มีจอภาพจึงยังสามารถควบคุมและแก้ไขโปรแกรมได้.

### 2. Explore — 30 นาที
ทำ Linux Mission และบันทึกผลจริง:
```bash
pwd
ls
ls -la
mkdir -p ~/robotics_lab/week02
cd ~/robotics_lab/week02
touch note.txt
cp note.txt note-copy.txt
mv note-copy.txt robot-note.txt
cat /etc/os-release
uname -m
hostnamectl
ip addr
```
อธิบายหน้าที่แต่ละคำสั่งด้วยภาษาของตนเอง.

### 3. Explain — 20 นาที
ครูสรุป filesystem, home directory, absolute/relative path, user/permission, package manager และความต่างของ `apt update` กับ `apt upgrade`.

### 4. Elaborate — 25 นาที
```bash
sudo apt update
sudo apt install git -y
git --version
systemctl --version
```
หากใช้ Pi ผ่านเครือข่าย ครูสาธิต:
```bash
sudo apt install openssh-server -y
sudo systemctl enable --now ssh
hostname -I
```
จากเครื่องลูก: `ssh <username>@<IP_RASPBERRY_PI>` ใช้เฉพาะเครือข่ายห้องเรียน ไม่เปิด port สู่ Internet.

### 5. Evaluate — 15 นาที
สร้าง `robotics/week02/evidence` และ `system-info.txt` ที่มี OS/architecture โดยนักเรียนเลือกคำสั่งเอง.

## ชิ้นงานและเกณฑ์
Terminal log + `system-info.txt` + Exit Ticket • Linux mission ≥80% • ความรู้ ≥70% • ใช้ `sudo` ได้อย่างมีเหตุผล.

## Challenge
ใช้ `ps` สำรวจ process และอธิบาย PID โดยไม่ terminate process ของระบบ.
