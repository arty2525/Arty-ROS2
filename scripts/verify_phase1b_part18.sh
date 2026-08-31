#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

cd "${ROOT}/firmware/esp32"
pio run

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

if grep -n "TCIFLUSH" \
  src/mobile_robot_hardware/src/mobile_robot_system.cpp; then
  echo "ERROR: RX flush remains in host frame recovery path"
  exit 1
fi

grep -q "FrameStreamParser" \
  src/mobile_robot_hardware/include/mobile_robot_hardware/protocol.hpp

grep -q "clear_firmware_faults" \
  src/mobile_robot_hardware/src/mobile_robot_system.cpp

echo "Phase1B-Part18 verification completed."
