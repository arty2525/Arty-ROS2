#!/usr/bin/env bash
set -euo pipefail
REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "${REPO_ROOT}/firmware/esp32"
pio run
source /opt/ros/jazzy/setup.bash
cd "${REPO_ROOT}/ros2_ws"
rosdep install --from-paths src --ignore-src --rosdistro jazzy -y
colcon build --packages-select mobile_robot_hardware --event-handlers console_direct+
colcon test --packages-select mobile_robot_hardware --event-handlers console_direct+
colcon test-result --verbose
