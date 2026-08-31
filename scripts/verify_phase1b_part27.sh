#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

grep -q "kOk = kAccepted" \
  "${ROOT}/ros2_ws/src/mobile_robot_hardware/include/mobile_robot_hardware/protocol.hpp"

if grep -q "2048.0F" \
  "${ROOT}/ros2_ws/src/mobile_robot_hardware/test/test_protocol.cpp"; then
  echo "ERROR: พบค่า encoder calibration ที่ไม่ได้ยืนยันใน protocol test" >&2
  exit 1
fi

grep -q "kSerializationSentinelBits" \
  "${ROOT}/ros2_ws/src/mobile_robot_hardware/test/test_protocol.cpp"

source /opt/ros/jazzy/setup.bash
cd "${ROOT}/ros2_ws"

rosdep install --from-paths src --ignore-src --rosdistro jazzy -y
colcon build --packages-select mobile_robot_hardware --symlink-install --event-handlers console_direct+
source install/setup.bash
colcon test --packages-select mobile_robot_hardware --event-handlers console_direct+
colcon test-result --verbose

echo "Phase1B-Part27 verification completed."
