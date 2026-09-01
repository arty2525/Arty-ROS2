# สัปดาห์ที่ 2 — Ubuntu/Linux สำหรับระบบหุ่นยนต์

**เวลา:** 100 นาที (2 × 50 นาที)

## เป้าหมาย
นักเรียนสามารถใช้ Terminal จัดการไฟล์ ตรวจระบบ ติดตั้ง package และเข้าใจ SSH เบื้องต้นได้

## คาบที่ 1
### Engage — 10 นาที
ถามว่าเหตุใดหุ่นยนต์ที่ไม่มีจอภาพจึงยังสามารถควบคุมและแก้ไขโปรแกรมได้.

### Explore — 30 นาที
ให้นักเรียนทำ Linux Mission ตามลำดับและบันทึกผลจริง:

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

นักเรียนอธิบายหน้าที่ของแต่ละคำสั่งด้วยภาษาของตนเอง.

### Explain — 10 นาที
ครูสรุป filesystem, home directory, absolute/relative path, user และ permission.

## คาบที่ 2
### Explain — 10 นาที
อธิบาย package manager และความแตกต่างระหว่าง `apt update` กับ `apt upgrade`.

### Elaborate — 25 นาที
ทดลอง:

```bash
sudo apt update
sudo apt install git -y
git --version
systemctl --version
```

หากห้องเรียนใช้ Raspberry Pi ผ่านเครือข่าย ให้ครูสาธิต SSH:

```bash
sudo apt install openssh-server -y
sudo systemctl enable --now ssh
hostname -I
```

จากเครื่องลูก:

```bash
ssh <username>@<IP_RASPBERRY_PI>
```

ห้ามนักเรียนเปิด port สู่ Internet; ใช้เฉพาะเครือข่ายห้องเรียนที่ครูกำหนด.

### Evaluate — 15 นาที
นักเรียนได้รับโจทย์สร้าง directory `robotics/week02/evidence` และสร้างไฟล์ `system-info.txt` ที่มีข้อมูล OS และ architecture โดยไม่ให้คำสั่งสำเร็จรูป.

## ชิ้นงาน
- Terminal log
- `system-info.txt`
- Exit Ticket

## เกณฑ์ผ่าน
- ทำ Linux mission ≥ 80%
- อธิบายคำสั่งหลักได้ ≥ 70%
- ไม่ใช้ `sudo` โดยไม่ทราบวัตถุประสงค์

## Challenge
ค้นหาว่า process ใดกำลังทำงานด้วย `ps` และหาความหมายของ PID โดยไม่ terminate process ของระบบ.
