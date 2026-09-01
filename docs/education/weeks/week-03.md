# สัปดาห์ที่ 3 — Git, GitHub และ Arty-ROS2 Repository

**เวลา:** 100 นาทีต่อเนื่อง

## เป้าหมาย
อธิบาย repository/commit/branch, clone โครงการ และใช้ `status`, `log`, `diff` ตรวจงานได้

## กิจกรรมการเรียนรู้ 100 นาทีต่อเนื่อง
### 1. Engage — 10 นาที
สถานการณ์: นักเรียน 5 คนแก้ source code คนละเครื่อง หากไม่มี version control จะรู้ได้อย่างไรว่าไฟล์ใดล่าสุดและใครเปลี่ยนอะไร?

### 2. Explore — 30 นาที
```bash
cd ~
git clone https://github.com/arty2525/Arty-ROS2.git
cd Arty-ROS2
git status
git log --oneline -10
git branch --show-current
```
สำรวจ repository และจัดหมวด firmware, ROS workspace, documentation, scripts.

### 3. Explain — 20 นาที
อธิบาย working tree → staging → commit → remote, commit SHA และเหตุผลที่ทุกกลุ่มต้องระบุ commit ที่ใช้ในการทดลองเพื่อ reproducibility.

### 4. Elaborate — 25 นาที
สร้าง local repository ฝึกนอก production source:
```bash
mkdir -p ~/robotics_lab/git-practice
cd ~/robotics_lab/git-practice
git init
printf 'Arty ROS2 Lab\n' > README.txt
git add README.txt
git status
git -c user.name="Student" -c user.email="student@example.invalid" commit -m "Add lab README"
printf 'Week 03\n' >> README.txt
git diff
```
ห้าม push credential หรือข้อมูลส่วนตัว.

### 5. Evaluate — 15 นาที
ส่ง `git status`, `git log -1 --oneline`, แผนผัง repository และ Git workflow diagram. ตอบว่า SHA มีประโยชน์อย่างไร, clone/pull ต่างกันอย่างไร และเหตุใด production source ต้องมี history.
