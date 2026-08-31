#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

source /opt/ros/jazzy/setup.bash
cd "${ROOT}/ros2_ws"

rosdep install --from-paths src --ignore-src --rosdistro jazzy -y

colcon build \
  --packages-up-to \
    mobile_robot_diagnostics \
    mobile_robot_control \
    mobile_robot_bringup \
  --symlink-install \
  --event-handlers console_direct+

source install/setup.bash

colcon test \
  --packages-select \
    mobile_robot_diagnostics \
    mobile_robot_bringup \
  --event-handlers console_direct+

colcon test-result --verbose

python3 -m py_compile \
  src/mobile_robot_diagnostics/mobile_robot_diagnostics/runtime_monitor.py \
  src/mobile_robot_diagnostics/launch/diagnostics.launch.py \
  src/mobile_robot_bringup/launch/robot.launch.py

grep -q "mobile_robot_diagnostics" src/mobile_robot_bringup/launch/robot.launch.py
grep -q "<exec_depend>ament_index_python</exec_depend>" src/mobile_robot_diagnostics/package.xml
grep -q "<exec_depend>launch_ros</exec_depend>" src/mobile_robot_diagnostics/package.xml

echo "Phase1B-Part19 verification completed."
