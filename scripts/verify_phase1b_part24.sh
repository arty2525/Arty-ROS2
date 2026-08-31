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
  --packages-select mobile_robot_hardware \
  --symlink-install \
  --event-handlers console_direct+

source install/setup.bash

colcon test \
  --packages-select mobile_robot_hardware \
  --event-handlers console_direct+

colcon test-result --verbose

grep -q \
  'mobile_robot_hardware/fault_flags.hpp' \
  src/mobile_robot_hardware/src/mobile_robot_system.cpp

grep -q \
  '"active_faults"' \
  src/mobile_robot_hardware/src/mobile_robot_system.cpp

grep -q \
  '"unknown_fault_bits"' \
  src/mobile_robot_hardware/src/mobile_robot_system.cpp

grep -q \
  'test_fault_flags' \
  src/mobile_robot_hardware/CMakeLists.txt

echo "Phase1B-Part24 verification completed."
