# คู่มือครู Week 06 — ROS 2 Workspace, Package และ Colcon

**เวลา:** 100 นาทีต่อเนื่อง  
**ใช้คู่กับ:** `docs/education/weeks/week-06.md`

## ความรู้ที่ครูควรทบทวน
ROS 2 Workspace คือพื้นที่รวม package ที่เราพัฒนา ส่วน package เป็นหน่วยโครงสร้างหลักของ source code, dependency, launch/config และ resource ต่าง ๆ

ใน workspace หลัง build มักเห็น 4 directory สำคัญ:
- `src/` source packages
- `build/` ไฟล์ระหว่างการ build
- `install/` ผลลัพธ์ที่ติดตั้งแบบ local ของ workspace
- `log/` log จาก colcon/build/test

`package.xml` เก็บ metadata และ dependency ของ package ส่วน `CMakeLists.txt` ใช้กับ package ที่ build ด้วย ament_cmake

## แนวคิดที่ต้องเน้น
อย่าให้นักเรียนคิดว่า “มี folder = เป็น ROS package” เพราะ package ต้องมี metadata/build structure ที่ ROS tools รู้จัก เช่น `package.xml`

`colcon build` ไม่ใช่ compiler โดยตรง แต่เป็น orchestration tool ที่จัดลำดับและเรียก build system ของหลาย package ตาม dependency

`rosdep` ช่วยติดตั้ง dependency ของระบบจาก declaration ใน package ไม่ควรแก้ error rosdep ด้วยการสุ่มติดตั้ง package ที่ชื่อคล้ายกัน

## เตรียมก่อนสอน
1. clone Arty-ROS2 ที่สถานะสะอาด
2. ตรวจ `colcon list`
3. ตรวจ package หลักที่อยู่ใน workspace
4. ทดลอง `rosdep install` และ `colcon build` บนเครื่องครู
5. เตรียม build log ที่มีทั้ง success และ error ตัวอย่าง

## ขั้นตอนสอน 100 นาที
### นาที 0–10: Engage
แสดง folder ที่มีไฟล์จำนวนมากกับ workspace ที่แบ่ง package แล้วถามว่า “ถ้าต้องแก้เฉพาะส่วน hardware เราจะหาไฟล์อย่างไร?”

### นาที 10–40: Explore
ใช้:
```bash
cd ~/Arty-ROS2/ros2_ws
colcon list
find src -maxdepth 2 -name package.xml -print
```
ให้นักเรียนจับคู่ชื่อ package กับหน้าที่ เช่น bringup, control, description, diagnostics, hardware

เปิด `package.xml` ของหนึ่ง package แล้วให้หา name/version/license/dependencies

### นาที 40–60: Explain
วาด dependency graph อย่างง่าย เช่น bringup อาจพึ่ง control/description/diagnostics แล้วอธิบายว่าถ้า package หนึ่งขาด metadata tools อาจมองไม่เห็นหรือแก้ dependency ผิด

อธิบาย build pipeline:
`source ROS underlay → rosdep → colcon build → source install overlay`

### นาที 60–85: Elaborate
ให้นักเรียนทำ:
```bash
source /opt/ros/jazzy/setup.bash
cd ~/Arty-ROS2/ros2_ws
rosdep install --from-paths src --ignore-src --rosdistro jazzy -y
colcon build --symlink-install
source install/setup.bash
ros2 pkg list | grep mobile_robot
```

ถ้า error ให้หยุดและจด “บรรทัดแรกที่บอกสาเหตุ” ก่อนแก้

### นาที 85–100: Evaluate
ให้นักเรียนวาด workspace structure และอธิบายว่าเหตุใด build แล้วต้อง source `install/setup.bash`

## จุดสับสนที่พบบ่อย
**`src` คือ source ของ Linux หรือ ROS ทั้งระบบ?** ไม่ใช่ เป็น source ของ workspace นี้

**ลบ `build/install/log` ได้ไหม?** โดยหลักสร้างใหม่ได้ แต่ไม่ควรให้เด็กใช้เป็นวิธีแก้ทุก errorโดยอัตโนมัติ ต้องเข้าใจสาเหตุก่อน

**`rosdep install` คือ build package หรือไม่?** ไม่ใช่ เป็นการติดตั้ง dependency จากระบบ package manager ตาม declaration

**package ไม่โผล่หลัง build** ตรวจว่า package ถูก discover, build ผ่าน และ source overlay แล้วหรือยัง

## วิธีสอนการอ่าน error
ให้ไล่จาก package ที่ failed → command ที่ failed → error แรกที่ meaningful → dependency/file/symbol ที่เกี่ยวข้อง หลีกเลี่ยงการคัดเฉพาะบรรทัดท้ายสุด

## เนื้อหาขยาย
อธิบาย topological build order และเหตุผลที่ dependency declaration ที่ถูกต้องมีผลต่อ CI และ reproducibility
