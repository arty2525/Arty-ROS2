#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

source /opt/ros/jazzy/setup.bash

cd "${ROOT}/ros2_ws"

rosdep install \
  --from-paths src \
  --ignore-src \
  --rosdistro jazzy \
  -y

colcon build \
  --packages-up-to \
    mobile_robot_description \
    mobile_robot_control \
    mobile_robot_bringup \
  --symlink-install \
  --event-handlers console_direct+

source install/setup.bash

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
  wheel_radius:=0.0325 \
  wheel_separation:=0.30 \
  wheel_width:=0.03 \
  base_length:=0.40 \
  base_width:=0.28 \
  base_height:=0.10 \
  caster_ball_radius:=0.012 \
  front_caster_offset:=0.15 \
  rear_caster_offset:=0.15 \
  > /tmp/arty_ros2_part16.urdf

grep -q 'name="left_wheel_joint"' /tmp/arty_ros2_part16.urdf
grep -q 'name="right_wheel_joint"' /tmp/arty_ros2_part16.urdf
grep -q 'name="front_caster_ball_joint"' /tmp/arty_ros2_part16.urdf
grep -q 'name="rear_caster_ball_joint"' /tmp/arty_ros2_part16.urdf
grep -q 'radius="0.0325"' /tmp/arty_ros2_part16.urdf

echo "Phase1B-Part16 verification completed."
