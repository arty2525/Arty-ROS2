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
  --packages-select \
    mobile_robot_hardware \
  --symlink-install \
  --event-handlers console_direct+

source install/setup.bash

colcon test \
  --packages-select \
    mobile_robot_hardware \
  --event-handlers console_direct+

colcon test-result --verbose

grep -q "setup_diagnostics" \
  src/mobile_robot_hardware/src/mobile_robot_system.cpp

grep -q "publish_hardware_diagnostics" \
  src/mobile_robot_hardware/src/mobile_robot_system.cpp

grep -q "diagnostic_fault_flags_" \
  src/mobile_robot_hardware/include/mobile_robot_hardware/mobile_robot_system.hpp

grep -q "find_package(diagnostic_msgs REQUIRED)" \
  src/mobile_robot_hardware/CMakeLists.txt

grep -q "<depend>diagnostic_msgs</depend>" \
  src/mobile_robot_hardware/package.xml

echo "Phase1B-Part20 verification completed."
