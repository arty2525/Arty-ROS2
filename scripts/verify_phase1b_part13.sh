#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "${ROOT}/firmware/esp32"
pio run
source /opt/ros/jazzy/setup.bash
cd "${ROOT}/ros2_ws"
colcon build --packages-select mobile_robot_hardware --symlink-install --event-handlers console_direct+
source install/setup.bash
colcon test --packages-select mobile_robot_hardware --event-handlers console_direct+
colcon test-result --verbose
