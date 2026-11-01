# คู่มือครูและแผนการจัดการเรียนรู้ — วิทยาการหุ่นยนต์ (1.5 หน่วยกิต)
**เวลาเรียน:** 60 คาบ × 50 นาที = 3,000 นาที (50 ชั่วโมง) • 20 สัปดาห์ • สัปดาห์ละ 3 คาบ แบ่งวันหนึ่ง 2 คาบ อีกวันหนึ่ง 1 คาบ • 20 สัปดาห์ • สัปดาห์ละ 3 คาบ แบ่งวันหนึ่ง 2 คาบ อีกวันหนึ่ง 1 คาบ

> เอกสารนี้เป็นต้นฉบับรายวิชาสำหรับปรับให้เข้ากับหลักสูตรสถานศึกษา ตัวชี้วัด/ผลการเรียนรู้ ช่วงชั้น และอุปกรณ์จริง ก่อนอนุมัติแผนอย่างเป็นทางการ

## 1. แนวคิดหลักและผลการเรียนรู้ที่คาดหวัง
ผู้เรียนสามารถ (1) อธิบายประวัติ ความหมาย และความสำคัญของวิทยาการหุ่นยนต์ (2) วิเคราะห์องค์ประกอบ Sensor, Controller, Actuator, Power, Communication (3) จำแนกชนิดตามโครงสร้าง การเคลื่อนที่ และภารกิจ (4) วิเคราะห์ความปลอดภัย จริยธรรม และการนำไปใช้ (5) สร้าง VM และติดตั้ง Ubuntu 22.04 ได้ (6) ติดตั้งและใช้ ROS 2 Humble, Nodes/Topics/Services/Workspace (7) ควบคุม Turtlesim ด้วย keyboard และโปรแกรม Python (8) ติดตั้ง Ubuntu/ROS บน Raspberry Pi 4 (9) สื่อสารกับ MCU/driver และควบคุมหุ่นยนต์จริงอย่างปลอดภัย (10) บันทึกผล ตรวจปัญหา และนำเสนอผลงานเชิงประจักษ์

## 2. โครงสร้างรายสัปดาห์และการจัด 5E
**หมายเหตุ:** ตารางสัปดาห์ 1–10 ด้านล่างเป็นแผนเดิมที่ขยายเป็น 20 สัปดาห์แล้ว ให้ยึดตาราง 20 สัปดาห์ฉบับปรับปรุงเป็นหลัก

**แนวทางต่อสัปดาห์:** คาบ 1–2 (100 นาที) = Engage 10 นาที, Explore 45 นาที, Explain 20 นาที, Elaborate 20 นาที, Evaluate 5 นาที; คาบ 3 (50 นาที) = ทบทวน 5 นาที, ฝึกปฏิบัติ 30 นาที, สะท้อนผล/ประเมิน 15 นาที. ปรับได้ตามภารกิจ ไม่จำเป็นต้องใช้ 5E แบบแข็งตัวทุกคาบ

| สัปดาห์ | คาบ | เนื้อหาหลัก | ผลงานและหลักฐานประเมิน |
|---|---|---|---|
| 1 | 1–3 | ประวัติ ความหมาย วิวัฒนาการ องค์ประกอบและประเภท | แผนภาพ Sense–Think–Act + timeline |
| 2 | 4–6 | การประยุกต์ ความปลอดภัย VMware Ubuntu 22.04 และคำสั่งเบื้องต้น | วิเคราะห์กรณีศึกษา + ภาพการสร้าง VM/คำสั่ง Terminal |
| 3 | 7–9 | เครือข่าย apt repository และติดตั้ง ROS 2 Humble | ภาพหน้าจอ ros2 --help และ log การติดตั้ง |
| 4 | 10–12 | Node Topic Service CLI และ Workspace | ผล topic list, echo, colcon build |
| 5 | 13–15 | Python package Publisher Subscriber Turtlesim | โค้ด node + ภาพการรัน Turtlesim |
| 6 | 16–18 | Keyboard Teleop Twist ผังงานควบคุม | ภาพเส้นทาง Turtlesim และคำสั่ง cmd_vel |
| 7 | 19–21 | โปรแกรม Python วาดรูปทรง / Debug และ Pi 4 | โค้ดควบคุม Turtlesim + เช็กลิสต์อุปกรณ์ |
| 8 | 22–24 | Ubuntu Server 22.04 ARM64, ROS 2 on Pi, UART/driver | บันทึก uname -m, network, SSH, wiring diagram |
| 9 | 25–27 | UART ACK, ROS bridge /cmd_vel และทดลอง teleop | log serial, เส้นทางทดสอบ, วิดีโอ |
| 10 | 28–30 | ความปลอดภัย ภารกิจหุ่นยนต์ ทดสอบหลังเรียน นำเสนอ | checklist, สาธิต, รายงานผลและ reflection |

## 2.1 ตารางรายสัปดาห์ฉบับปรับปรุง 20 สัปดาห์

> ทุกสัปดาห์แบ่งวันที่ 1 จำนวน 2 คาบ (100 นาที) และวันที่ 2 จำนวน 1 คาบ (50 นาที) รวม 60 คาบ; เนื้อหาขยายจากโครงร่างเดิมโดยเพิ่มคาบฝึกปฏิบัติและบันทึกผลจริง

| สัปดาห์ | คาบ | รายการเรียนรู้และฝึกปฏิบัติ |
|---|---|---|
| 1 | 1–3 | ประวัติและความหมายของหุ่นยนต์ / วิวัฒนาการและองค์ประกอบ |
| 2 | 4–6 | วิวัฒนาการและองค์ประกอบ / ประเภทของหุ่นยนต์ |
| 3 | 7–9 | การนำไปใช้และความปลอดภัย / VMware / Ubuntu เตรียมติดตั้ง |
| 4 | 10–12 | VMware / Ubuntu เตรียมติดตั้ง / Ubuntu Terminal และระบบไฟล์ |
| 5 | 13–15 | แพ็กเกจและเครือข่าย Linux / เตรียม ROS 2 repository |
| 6 | 16–18 | เตรียม ROS 2 repository / ติดตั้ง ROS 2 Humble |
| 7 | 19–21 | ROS graph, Node, Topic / Service, Message, CLI |
| 8 | 22–24 | Service, Message, CLI / สร้าง workspace |
| 9 | 25–27 | สร้าง Python package / Publisher / Subscriber |
| 10 | 28–30 | Publisher / Subscriber / Turtlesim พื้นฐาน |
| 11 | 31–33 | Turtlesim Keyboard / Twist และความเร็ว |
| 12 | 34–36 | Twist และความเร็ว / ออกแบบลำดับการเคลื่อนที่ |
| 13 | 37–39 | เขียน Python rclpy / Debug และภารกิจจำลอง |
| 14 | 40–42 | Debug และภารกิจจำลอง / แนะนำ Raspberry Pi 4 |
| 15 | 43–45 | ติดตั้ง Ubuntu บน Pi / ROS 2 บน Raspberry Pi 4 |
| 16 | 46–48 | ROS 2 บน Raspberry Pi 4 / โครงสร้างควบคุมมอเตอร์ |
| 17 | 49–51 | การเชื่อมต่อ UART / ROS /cmd_vel ไปยังมอเตอร์ |
| 18 | 52–54 | ROS /cmd_vel ไปยังมอเตอร์ / Keyboard Teleop หุ่นยนต์จริง |
| 19 | 55–57 | ความปลอดภัยและทดสอบจริง / ภารกิจหุ่นยนต์ |
| 20 | 58–60 | ภารกิจหุ่นยนต์ / ทดสอบหลังเรียนและนำเสนอ |

## 3. เกณฑ์ประเมินรายวิชาที่เสนอ (ปรับตามสถานศึกษา)
- แบบทดสอบก่อนเรียน: ใช้วินิจฉัยพื้นฐาน **ไม่คิดคะแนนตัดสิน**
- ใบกิจกรรม 20 หน่วย: 30 คะแนน (ความถูกต้อง ขั้นตอน และหลักฐาน)
- ภารกิจปฏิบัติ ROS/Turtlesim: 25 คะแนน (ติดตั้ง ใช้ topic สร้างโปรแกรม)
- โครงงานหุ่นยนต์จริง: 25 คะแนน (ความปลอดภัย การควบคุม การแก้ปัญหา และสาธิต)
- แบบทดสอบหลังเรียน: 20 คะแนน (ปรนัย 20 ข้อ)
- พฤติกรรมและคุณลักษณะ: บันทึกแยกเป็นแบบประเมินรายบุคคล ไม่คิดรวมโดยอัตโนมัติจนกว่าครูจะกำหนดน้ำหนักและเกณฑ์ของโรงเรียน
รวมคะแนนผลสัมฤทธิ์เสนอ = 100 คะแนน

### แบบสังเกตพฤติกรรมการทำงาน (คะแนนเต็ม 15)
1. แสดงความคิดเห็นโดยมีเหตุผล 2. ยอมรับฟังผู้อื่น 3. ทำงานตามหน้าที่ 4. รับผิดชอบและตรงต่อเวลา 5. ใช้เทคโนโลยีอย่างเหมาะสม
3 = ทำได้ชัดเจนและสม่ำเสมอ; 2 = ทำได้เป็นส่วนใหญ่; 1 = ยังต้องมีการกระตุ้น/ช่วยเหลือ; ประเมินตามหลักฐานที่สังเกตจริง ห้ามเติม 3 ทุกข้ออัตโนมัติ

### แบบประเมินคุณลักษณะ (คะแนนเต็ม 18)
1. มีวินัย 2. ใฝ่เรียนรู้ 3. มุ่งมั่นในการทำงาน 4. รับผิดชอบ 5. รับฟังผู้อื่น 6. ใช้เทคโนโลยีอย่างเหมาะสม
เกณฑ์รายข้อ 3/2/1 โดยครูเป็นผู้ประเมินเท่านั้น ห้ามคัดลอกค่าจากวิชาอื่นหรือจำลองคะแนนรายบุคคล

## 4. ระบบและอุปกรณ์
- PC x86-64: Ubuntu 22.04 AMD64 ใน VMware Fusion/Workstation ตามแพลตฟอร์มที่รองรับ
- Mac Apple Silicon: Ubuntu 22.04 **ARM64** ใน VMware Fusion ที่รองรับ Apple Silicon; ไม่ใช่ ISO AMD64; โปรแกรมภาพ 3D อาจมีข้อจำกัด graphics acceleration
- Raspberry Pi 4: Ubuntu Server 22.04 **ARM64**, microSD คุณภาพดี, power supply 5V 3A ที่มีเสถียรภาพ, network, SSH
- หุ่นยนต์: Pi 4, ESP32 หรือ microcontroller ตามฮาร์ดแวร์จริง, motor driver ที่เหมาะสม, encoder ถ้ามี, แบตเตอรี่และระบบหยุดฉุกเฉิน
- ถ้าใช้มอเตอร์สี่ล้อ: การสั่งเดินหน้า/ถอยต้องทดสอบขั้วและทิศทีละชุดก่อน ไม่ถือว่า pin configuration ใช้ร่วมกันทุกเครื่องได้

## 5. ติดตั้ง Ubuntu และ ROS 2 Humble (ใน VM และ Pi)
คู่มืออ้างอิงหลัก: https://docs.ros.org/en/humble/Installation/Ubuntu-Install-Debs.html
ROS 2 Humble ได้รับแพ็กเกจ Ubuntu Jammy 22.04 ทั้ง amd64 และ arm64 ตามแพลตฟอร์มที่รองรับ; ตรวจรุ่นและสถาปัตยกรรมด้วย:
```bash
lsb_release -a
uname -m
sudo apt update && sudo apt upgrade -y
sudo apt install -y software-properties-common curl git
sudo add-apt-repository universe
```
**เพิ่ม ROS 2 apt source ตามคู่มือทางการที่เชื่อมข้างต้น** เพราะวิธีเพิ่ม source/keyring อาจมีการอัปเดต ไม่ควรใช้คีย์จาก URL เก่าที่ไม่ตรวจสอบ แล้วติดตั้ง:
```bash
sudo apt update
# บน VM เดสก์ท็อป
sudo apt install -y ros-humble-desktop ros-dev-tools
# บน Pi แบบไม่มี GUI ใช้แทนด้วย:
# sudo apt install -y ros-humble-ros-base ros-dev-tools
source /opt/ros/humble/setup.bash
ros2 --help
```
เพิ่ม source ใน shell โดยตั้งใจและหลังตรวจสอบแล้ว: `echo 'source /opt/ros/humble/setup.bash' >> ~/.bashrc`

## 6. ROS graph และ workspace
Terminal 1:
```bash
source /opt/ros/humble/setup.bash
ros2 run demo_nodes_cpp talker
```
Terminal 2:
```bash
source /opt/ros/humble/setup.bash
ros2 run demo_nodes_py listener
ros2 node list
ros2 topic list
ros2 topic echo /chatter
```
สร้าง workspace/package:
```bash
mkdir -p ~/ros2_ws/src
cd ~/ros2_ws/src
ros2 pkg create --build-type ament_python --license Apache-2.0 turtle_shapes --dependencies rclpy geometry_msgs
cd ~/ros2_ws
colcon build
source install/setup.bash
```

## 7. Turtlesim จาก keyboard สู่ Python
Terminal 1: `ros2 run turtlesim turtlesim_node`
Terminal 2: `ros2 run turtlesim turtle_teleop_key` แล้วคลิก Terminal 2 และใช้ปุ่มลูกศร

ทดลองข้อความ Twist:
```bash
ros2 topic pub --once /turtle1/cmd_vel geometry_msgs/msg/Twist "{linear: {x: 2.0}, angular: {z: 0.0}}"
```

**ตัวอย่าง Python Node ควบคุม Turtlesim** สร้างไฟล์ `~/ros2_ws/src/turtle_shapes/turtle_shapes/square.py`
```python
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist

class Square(Node):
    def __init__(self):
        super().__init__('turtle_square')
        self.pub = self.create_publisher(Twist, '/turtle1/cmd_vel', 10)
        self.count = 0
        self.timer = self.create_timer(0.1, self.tick)

    def tick(self):
        msg = Twist()
        # เดินหน้า 2 วินาที แล้วหมุนประมาณ 90 องศา
        phase = self.count % 35
        if phase < 20:
            msg.linear.x = 1.6
        else:
            msg.angular.z = 1.05
        if self.count >= 4 * 35:
            self.pub.publish(Twist())
            self.timer.cancel()
            self.get_logger().info('Complete')
            return
        self.pub.publish(msg)
        self.count += 1

def main(args=None):
    rclpy.init(args=args)
    node = Square()
    try:
        rclpy.spin(node)
    finally:
        node.pub.publish(Twist())
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()
```
เพิ่มใน `setup.py` ของแพ็กเกจ:
```python
entry_points={
    'console_scripts': [
        'square = turtle_shapes.square:main',
    ],
},
```
จากนั้น `cd ~/ros2_ws && colcon build --packages-select turtle_shapes && source install/setup.bash && ros2 run turtle_shapes square`.
**หมายเหตุ:** มุมหมุนขึ้นอยู่กับอัตราจริงของ Turtlesim และจังหวะ timer; โค้ดเป็นงานเริ่มต้นให้ผู้เรียนทดลองปรับค่า ไม่รับประกันรูปทรงสี่เหลี่ยมสมบูรณ์

## 8. เส้นทางสู่หุ่นยนต์จริง
สถาปัตยกรรมเป้าหมาย:
```
Keyboard Teleop / Python Node (ROS2)
                  |
               /cmd_vel  (geometry_msgs/Twist)
                  |
      Raspberry Pi 4 ROS2 bridge
                  |
          Serial UART 115200
                  |
         ESP32 motor controller
                  |
      Motor Driver -> DC Motors
                  ^
       Encoder feedback (ถ้ามี)
```
**เงื่อนไขความปลอดภัยก่อนจ่ายไฟให้ล้อ**
1. ตรวจแรงดัน กระแสสูงสุดและขั้วมอเตอร์ / driver ตาม datasheet
2. แยกการจ่ายไฟภาคมอเตอร์และ Pi ตามการออกแบบไฟฟ้าที่เหมาะสม; จัดกราวด์สัญญาณร่วมเมื่อจำเป็นและปลอดภัย
3. ตั้ง watchdog: หากไม่ได้รับคำสั่งใหม่ภายในเวลาที่กำหนด ESP32 ต้องสั่ง PWM = 0
4. คำสั่งหยุดต้องได้ผลแม้สาย USB/UART หรือเครือข่ายหลุด; ห้ามพึ่งพาคำสั่งหยุดทางซอฟต์แวร์เพียงอย่างเดียว
5. ทดลองยกล้อพ้นพื้น ใช้ความเร็วต่ำ กำหนดพื้นที่ปลอดภัยก่อนวางลงพื้น
6. บันทึกภาพหลักฐาน wiring, output log, test procedure และสาเหตุข้อผิดพลาด ไม่ระบุว่าทดสอบสำเร็จก่อนตรวจจริง

## 9. เว็บไซต์/การจัดเก็บคะแนน
- Portal: `/robotics-course/index.html`
- 3D VR Lab: `/robotics-course/vr.html`
- สมุดประเมินส่วนกลางสำหรับครู: `/robotics-course/gradebook.html` ใช้รหัสครูเดียวกับระบบประเมินเดิม แยกตารางฐานข้อมูล `robotics_assessments` ที่เปิด RLS และปิดการอ่าน/เขียนสาธารณะ
- แบบทดสอบก่อน–หลังเรียนของผู้เรียน **ยังเก็บเฉพาะในเบราว์เซอร์** สามารถส่งออก CSV/สำรอง JSON ได้ การเชื่อมระบบบัญชีนักเรียนและฐานข้อมูลผลสอบเป็นงานเพิ่มเติม
- ห้ามกรอกเลขประจำตัวประชาชนในช่องรหัสนักเรียนบนเว็บไซต์สาธารณะ

## 10. แนวทางจัดการแผนและหลักฐานตาม PA/DPA
รวบรวม (ก) แผนที่ได้รับอนุมัติ (ข) pre/post ที่ตรวจสอบตัวตนผู้เรียนได้ (ค) ใบงานและภาพปฏิบัติจริง (ง) ผลประเมินพฤติกรรมและคุณลักษณะที่มีหลักฐาน (จ) ภาพ/วิดีโอการควบคุม Turtlesim และหุ่นยนต์จริง (ฉ) การสะท้อนผลและการปรับปรุงกิจกรรม โดยไม่จัดทำข้อมูลย้อนหลังหรือกล่าวอ้างผลสำเร็จที่ยังไม่มีหลักฐาน
