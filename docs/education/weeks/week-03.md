# สัปดาห์ที่ 3 — Git, GitHub และ Arty-ROS2 Repository

**เวลา:** 100 นาที

## เป้าหมาย
นักเรียนสามารถอธิบาย repository/commit/branch, clone โครงการ และใช้ `status`, `log`, `diff` เพื่อตรวจงานได้.

## คาบที่ 1
### Engage — 10 นาที
สถานการณ์: นักเรียน 5 คนแก้ source code คนละเครื่อง หากไม่มี version control จะรู้ได้อย่างไรว่าไฟล์ใดล่าสุดและใครเปลี่ยนอะไร?

### Explore — 30 นาที
```bash
cd ~
git clone https://github.com/arty2525/Arty-ROS2.git
cd Arty-ROS2
git status
git log --oneline -10
git branch --show-current
```

ให้นักเรียนสำรวจ repository ด้วย `ls`/`find` และจัดหมวด firmware, ROS workspace, documentation, scripts.

### Explain — 10 นาที
ครูอธิบาย working tree → staging → commit → remote และความหมายของ commit SHA.

## คาบที่ 2
### Explain — 10 นาที
อธิบายเหตุผลที่ทุกกลุ่มต้องระบุ commit ที่ใช้ในการทดลอง เพื่อ reproducibility.

### Elaborate — 25 นาที
นักเรียนสร้างไฟล์ฝึกนอก production source เช่น `~/robotics_lab/git-practice.txt`, สร้าง local repository และทดลอง add/commit/diff.

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

ไม่ push credential หรือข้อมูลส่วนตัวขึ้น repository.

### Evaluate — 15 นาที
นักเรียนส่ง screenshot/log ของ `git status`, `git log -1 --oneline` และวาด workflow Git 1 ภาพ.

## ชิ้นงาน
- หลักฐาน clone Arty-ROS2
- แผนผังโครงสร้าง repository
- Git workflow diagram

## คำถามท้ายกิจกรรม
1. Commit SHA มีประโยชน์ต่อการทดลองหุ่นยนต์อย่างไร?
2. `git pull` ต่างจาก `git clone` อย่างไร?
3. เหตุใดจึงไม่ควรแก้ production source โดยไม่มี commit history?
