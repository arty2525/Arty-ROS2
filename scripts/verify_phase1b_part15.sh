#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

echo "== ESP32 firmware =="
cd "${ROOT}/firmware/esp32"
pio run

echo "== ROS 2 =="
source /opt/ros/jazzy/setup.bash
cd "${ROOT}/ros2_ws"

rosdep install \
  --from-paths src \
  --ignore-src \
  --rosdistro jazzy \
  -y

colcon build \
  --packages-up-to \
    mobile_robot_hardware \
    mobile_robot_description \
    mobile_robot_control \
    mobile_robot_bringup \
  --symlink-install \
  --event-handlers console_direct+

source install/setup.bash

colcon test \
  --packages-select mobile_robot_hardware \
  --event-handlers console_direct+

colcon test-result --verbose

echo "== Xacro validation =="

xacro \
  "$(ros2 pkg prefix mobile_robot_description)/share/mobile_robot_description/urdf/mobile_robot.urdf.xacro" \
  serial_device:=/dev/serial0 \
  baud_rate:=115200 \
  telemetry_timeout_ms:=500 \
  ticks_per_revolution:=2048 \
  left_motor_inverted:=false \
  right_motor_inverted:=false \
  left_encoder_inverted:=false \
  right_encoder_inverted:=false \
  wheel_radius:=0.05 \
  wheel_separation:=0.30 \
  wheel_width:=0.04 \
  base_length:=0.35 \
  base_width:=0.28 \
  base_height:=0.12 \
  > /tmp/arty_ros2_part15.urdf

grep -q "telemetry_timeout_ms" /tmp/arty_ros2_part15.urdf
grep -q "left_wheel_joint" /tmp/arty_ros2_part15.urdf
grep -q "right_wheel_joint" /tmp/arty_ros2_part15.urdf

echo "Phase1B-Part15 verification completed."
