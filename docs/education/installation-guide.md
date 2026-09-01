# คู่มือติดตั้ง Arty-ROS2 สำหรับห้องเรียน

## 1. เป้าหมาย

ติดตั้ง Raspberry Pi 4 ด้วย Ubuntu 24.04 LTS 64-bit ARM, ROS 2 Jazzy, Git, rosdep/colcon และ PlatformIO เพื่อใช้งาน repository `arty2525/Arty-ROS2` กับ ESP32 และฐาน Differential Drive

## 2. ติดตั้ง Ubuntu 24.04 LTS

1. เปิด Raspberry Pi Imager บน PC
2. Device: Raspberry Pi 4
3. OS: Ubuntu 24.04 LTS 64-bit สำหรับ Raspberry Pi
4. Storage: microSD อย่างน้อย 16 GB (แนะนำ 32 GB ขึ้นไป)
5. ตั้ง hostname, user/password, Wi-Fi, timezone `Asia/Bangkok` และ SSH ตามความเหมาะสม
6. Write + Verify
7. ใส่ microSD ใน Pi แล้ว First Boot

ตรวจระบบ:

```bash
sudo apt update
sudo apt full-upgrade -y
sudo reboot
uname -m
cat /etc/os-release
```

## 3. SSH

```bash
sudo apt install openssh-server -y
sudo systemctl enable --now ssh
hostname -I
```

จาก Notebook:

```bash
ssh <username>@<IP_RASPBERRY_PI>
```

## 4. ROS 2 Jazzy

```bash
sudo apt update
sudo apt install locales software-properties-common curl -y
sudo locale-gen en_US en_US.UTF-8
sudo update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
export LANG=en_US.UTF-8
sudo add-apt-repository universe

export ROS_APT_SOURCE_VERSION=$(curl -s https://api.github.com/repos/ros-infrastructure/ros-apt-source/releases/latest | grep -F "tag_name" | awk -F'"' '{print $4}')
curl -L -o /tmp/ros2-apt-source.deb "https://github.com/ros-infrastructure/ros-apt-source/releases/download/${ROS_APT_SOURCE_VERSION}/ros2-apt-source_${ROS_APT_SOURCE_VERSION}.$(. /etc/os-release && echo ${UBUNTU_CODENAME:-${VERSION_CODENAME}})_all.deb"
sudo dpkg -i /tmp/ros2-apt-source.deb
sudo apt update
sudo apt install ros-jazzy-desktop ros-dev-tools -y

echo 'source /opt/ros/jazzy/setup.bash' >> ~/.bashrc
source ~/.bashrc
printenv ROS_DISTRO
```

ทดสอบ Terminal 1:

```bash
ros2 run demo_nodes_cpp talker
```

Terminal 2:

```bash
ros2 run demo_nodes_py listener
```

## 5. Clone และ Build Arty-ROS2

```bash
sudo apt install git -y
cd ~
git clone https://github.com/arty2525/Arty-ROS2.git
cd ~/Arty-ROS2/ros2_ws
source /opt/ros/jazzy/setup.bash
colcon list
sudo rosdep init || true
rosdep update
rosdep install --from-paths src --ignore-src --rosdistro jazzy -y
colcon build --symlink-install
source install/setup.bash
colcon test
colcon test-result --verbose
```

## 6. PlatformIO และ ESP32

```bash
sudo apt install python3-venv curl -y
cd ~
curl -fsSL -o get-platformio.py https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py
python3 get-platformio.py
mkdir -p ~/.local/bin
ln -sf ~/.platformio/penv/bin/platformio ~/.local/bin/platformio
ln -sf ~/.platformio/penv/bin/pio ~/.local/bin/pio
echo 'export PATH="$HOME/.local/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc
pio --version
```

Build firmware:

```bash
cd ~/Arty-ROS2/firmware/esp32
pio run
```

Upload:

```bash
pio device list
pio run -t upload
```

หากไม่มีสิทธิ์ Serial:

```bash
sudo usermod -aG dialout $USER
sudo reboot
```

## 7. UART และ GPIO ที่กำหนดในโครงการ

### Raspberry Pi ↔ ESP32

- Pi TX GPIO14 → ESP32 RX2 GPIO16
- Pi RX GPIO15 ← ESP32 TX2 GPIO17
- UART 115200 baud

### ESP32

- PWM_LEFT GPIO25
- DIR_LEFT GPIO26
- PWM_RIGHT GPIO27
- DIR_RIGHT GPIO14
- Encoder Left A GPIO32
- Encoder Left B GPIO33
- Encoder Right A GPIO34
- Encoder Right B GPIO35

GPIO34/35 เป็น input-only และไม่มี internal pull-up ต้องตรวจชนิดสัญญาณ Encoder ก่อนต่อจริง

## 8. ค่าที่ห้ามเดา

ค่าต่อไปนี้ต้องวัด/ทดสอบจากหุ่นจริงก่อนใช้:

- wheel separation
- wheel width
- base dimensions
- caster radius/offsets
- encoder ticks per revolution
- motor/encoder inversion
- PID
- motion limits

ค่าที่ยืนยันแล้ว: ล้อขับเส้นผ่านศูนย์กลาง 65 mm ดังนั้น `wheel_radius = 0.0325 m`

## 9. Safety Checklist ก่อน Motor Test

- ตรวจสายและ polarity
- ตรวจแรงดัน logic
- TX ต่อ RX และ RX ต่อ TX
- ยกล้อขับให้พ้นพื้น
- เริ่มความเร็วต่ำ
- มีผู้ควบคุมคำสั่งเพียงคนเดียว
- มีวิธีตัดไฟฉุกเฉิน
- หยุดทันทีเมื่อพฤติกรรมผิดปกติ
- บันทึก commit/config ที่ใช้ทุกครั้ง
