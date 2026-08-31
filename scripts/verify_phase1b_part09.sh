#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
source /opt/ros/jazzy/setup.bash
cd "${ROOT}/ros2_ws"
rosdep install --from-paths src --ignore-src --rosdistro jazzy -y
colcon build --packages-up-to mobile_robot_diagnostics mobile_robot_bringup --symlink-install --event-handlers console_direct+
source install/setup.bash
colcon test --packages-select mobile_robot_diagnostics --event-handlers console_direct+
colcon test-result --verbose
