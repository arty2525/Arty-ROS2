# คู่มือครู Week 03 — Git, GitHub และการควบคุมเวอร์ชัน

**เวลา:** 100 นาทีต่อเนื่อง  
**บทนักเรียนที่ใช้คู่กัน:** `docs/education/weeks/week-03.md`

## 1. ความรู้ที่ครูควรทบทวนก่อนสอน
Git คือระบบควบคุมเวอร์ชันแบบ distributed ส่วน GitHub คือบริการ hosting repository และเครื่องมือทำงานร่วมกันบน Git ครูควรแยกสองคำนี้ให้ชัด เพราะนักเรียนมักใช้แทนกัน

คำสำคัญ:
- Repository: โครงการที่ Git ติดตาม
- Working tree: ไฟล์ที่กำลังทำงาน
- Staging area: พื้นที่เตรียมสิ่งที่จะ commit
- Commit: snapshot ของการเปลี่ยนแปลงพร้อม metadata
- Branch: เส้นทางการพัฒนา
- Remote: repository อีกตำแหน่ง เช่น GitHub
- Clone: สร้าง local repository จาก remote
- Pull: ดึงและรวมการเปลี่ยนแปลงจาก remote
- Diff: เปรียบเทียบการเปลี่ยนแปลง
- SHA: รหัสอ้างอิง commit

## 2. แนวคิดที่ต้องอธิบายให้ชัด
Git ไม่ได้มีไว้ “เก็บไฟล์ออนไลน์” อย่างเดียว แต่ช่วยตอบคำถาม 4 ข้อ:
1. อะไรเปลี่ยน?
2. ใครเปลี่ยน?
3. เมื่อไร?
4. กลับไปสถานะเดิมได้อย่างไร?

สำหรับงานหุ่นยนต์ มีข้อเพิ่มคือ “การทดลองนี้ใช้ source code เวอร์ชันใด?” ดังนั้น commit SHA จึงเป็นหลักฐานสำคัญของ reproducibility

## 3. เตรียมก่อนสอน
1. ตรวจว่า `git --version` ทำงาน
2. ตรวจ Internet หรือเตรียม clone ไว้ล่วงหน้า
3. เตรียม repository `arty2525/Arty-ROS2`
4. เตรียมโฟลเดอร์ฝึกแยกจาก production repository
5. เตรียมตัวอย่าง `git diff` ที่เห็นชัดหนึ่งไฟล์

## 4. อธิบาย workflow แบบภาพ
วาดลำดับ:
`Edit → git status → git diff → git add → git commit → optional push`

ย้ำว่า `git add` ไม่ได้ upload ขึ้น GitHub และ `git commit` ก็ยังเป็น local จนกว่าจะ push

## 5. ลำดับสอน 100 นาที
### นาที 0–10 — Engage
สถานการณ์: นักเรียนสองคนแก้ไฟล์เดียวกันแล้วส่งผ่าน LINE/USB ครูถาม “จะรู้ได้อย่างไรว่าไฟล์ไหนล่าสุด?” แล้วค่อยพาไปสู่ version control

### นาที 10–40 — Explore
ให้นักเรียน clone Arty-ROS2 แล้วใช้:
```bash
cd ~
git clone https://github.com/arty2525/Arty-ROS2.git
cd Arty-ROS2
git status
git log --oneline -10
git branch --show-current
```

ครูชี้ให้เห็นว่า clone แล้วได้ทั้งไฟล์และ history

### นาที 40–60 — Explain
อธิบาย commit SHA, branch, remote และ reproducibility ในงานวิศวกรรม ยกตัวอย่างว่า “หุ่นวิ่งผิดหลัง update” เราสามารถเทียบ commit ก่อน/หลังได้

### นาที 60–85 — Elaborate
ให้ทำ local repository ฝึกนอก production source และลอง edit/add/commit/diff โดยเน้นให้ดู `git status` ทุกขั้น

ตัวอย่าง:
```bash
mkdir -p ~/robotics_lab/git-practice
cd ~/robotics_lab/git-practice
git init
printf 'Robot Lab\n' > README.txt
git status
git add README.txt
git status
git -c user.name="Student" -c user.email="student@example.invalid" commit -m "Add README"
printf 'Week 03\n' >> README.txt
git diff
```

### นาที 85–100 — Evaluate
ให้นักเรียนวาด workflow และอธิบายด้วยตนเองว่า staging/commit/remote ต่างกันอย่างไร

## 6. จุดที่นักเรียนมักสับสน
### Git กับ GitHub
Git = เครื่องมือ version control; GitHub = platform ที่ host Git repository

### Clone กับ Download ZIP
ZIP ให้ไฟล์อย่างเดียว ไม่มี Git history และ remote configuration แบบ clone

### Commit กับ Save
Save บันทึกไฟล์ปัจจุบัน; commit บันทึก snapshot ของชุดการเปลี่ยนแปลงใน repository พร้อมข้อความและผู้ทำ

### Pull กับ Clone
Clone ใช้สร้าง local repo ครั้งแรก; pull ใช้ update repo ที่มีอยู่แล้ว

## 7. ปัญหาที่พบบ่อย
**`fatal: not a git repository`** — อยู่ผิด directory ให้ใช้ `pwd`/`ls -la` ตรวจ `.git`  
**nothing to commit** — ไม่มีการเปลี่ยนแปลงใหม่หรือยังแก้ไฟล์ไม่สำเร็จ  
**identity unknown** — Git ต้องการ user.name/email สำหรับ commit; ในห้องเรียนใช้ค่าที่ครูกำหนด  
**merge conflict** — ยังไม่จำเป็นต้องลงลึกในสัปดาห์นี้ แต่ให้รู้ว่าเกิดเมื่อ Git รวมการแก้ไฟล์เดียวกันอัตโนมัติไม่ได้

## 8. หลักความปลอดภัยข้อมูล
ห้าม commit password, API key, Wi-Fi credential หรือข้อมูลส่วนตัวลง repository. ครูควรอธิบายว่า “ลบไฟล์ภายหลัง” ไม่ได้แปลว่าความลับหายจาก Git history เสมอ

## 9. คำถามชี้นำ
- ทำไมต้องบันทึก commit SHA ก่อนทดลองหุ่น? → เพื่อรู้ software baseline และทำซ้ำได้
- `git status` ควรใช้เมื่อไร? → ก่อน/หลังการแก้และก่อน commit เพื่อรู้สถานะไฟล์
- ทำไมไม่แก้ production source แบบไม่มี commit? → ย้อนกลับและวิเคราะห์สาเหตุยาก

## 10. เนื้อหาขยาย
สำหรับนักเรียนเร็ว ให้สาธิต branch concept ด้วยกระดาน: main = เส้นหลัก, feature branch = เส้นทดลอง แล้ว merge หลังตรวจสอบ ไม่จำเป็นต้องให้ push จริงในสัปดาห์แรกของ Git
