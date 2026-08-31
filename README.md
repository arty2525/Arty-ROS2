# ROS2-Mobile-Robot

[![ROS 2](https://img.shields.io/badge/ROS%202-Jazzy-22314E.svg)](https://docs.ros.org/en/jazzy/)
[![Ubuntu](https://img.shields.io/badge/Ubuntu-24.04%20LTS-E95420.svg)](https://ubuntu.com/)
[![Platform](https://img.shields.io/badge/Platform-Raspberry%20Pi%204-C51A4A.svg)](https://www.raspberrypi.com/products/raspberry-pi-4-model-b/)
[![License](https://img.shields.io/badge/License-Apache--2.0-blue.svg)](LICENSE)

Repository สำหรับพัฒนาหุ่นยนต์เคลื่อนที่แบบ Differential Drive ระดับ Production โดยใช้ Raspberry Pi 4, Ubuntu 24.04 LTS, ROS 2 Jazzy, ESP32, Cytron MDD10A และมอเตอร์พร้อม Encoder

## Hardware

- Raspberry Pi 4 + Ubuntu 24.04 LTS + ROS 2 Jazzy
- NodeMCU ESP32S V1.1 / ESP-WROOM-32
- Cytron MDD10A Rev2.0
- Cytron TG42E-24K 12V 248RPM พร้อม Quadrature Encoder
- Differential Drive พร้อม Closed-loop wheel velocity control

## Software architecture

- `firmware/esp32` — ESP32 motor/encoder firmware และ UART protocol
- `ros2_ws/src/mobile_robot_hardware` — ros2_control SystemInterface
- `ros2_ws/src/mobile_robot_control` — controller_manager และ diff_drive_controller
- `ros2_ws/src/mobile_robot_description` — URDF/Xacro
- `docs` — เอกสารโครงการภาษาไทย
- `hardware` — pinout, wiring และข้อกำหนดความปลอดภัย

## Current status

Phase 1A เสร็จแล้ว และ Phase 1B อยู่ระหว่างพัฒนาระบบ hardware integration, UART, ros2_control และ differential-drive control

ค่าทางกายภาพที่ยังไม่ได้ยืนยัน เช่น wheel radius, wheel separation และ encoder counts per revolution จะไม่ถูกคาดเดา ระบบ Production จะปฏิเสธการเปิดใช้งานเมื่อค่าที่จำเป็นยังไม่ถูกตั้งอย่างถูกต้อง

## License

Apache License 2.0
