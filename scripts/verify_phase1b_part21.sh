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

grep -q "MatchesFirmwareBitLayout" \
  src/mobile_robot_hardware/test/test_fault_flags.cpp

grep -q "unknown_fault_bits" \
  ../docs/software/esp32-fault-flags.md

echo "Phase1B-Part21 verification completed."
