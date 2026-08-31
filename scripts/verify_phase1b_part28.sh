#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
WORKSPACE="${ROOT}/ros2_ws"

source /opt/ros/jazzy/setup.bash

cd "${WORKSPACE}"

test -f src/mobile_robot_control/package.xml
test -f src/mobile_robot_control/CMakeLists.txt

COLCON_PACKAGES="$(colcon list --names-only)"

grep -qx "mobile_robot_control" <<< "${COLCON_PACKAGES}"
grep -qx "mobile_robot_bringup" <<< "${COLCON_PACKAGES}"
grep -qx "mobile_robot_description" <<< "${COLCON_PACKAGES}"
grep -qx "mobile_robot_diagnostics" <<< "${COLCON_PACKAGES}"
grep -qx "mobile_robot_hardware" <<< "${COLCON_PACKAGES}"

rosdep install \
  --from-paths src \
  --ignore-src \
  --rosdistro jazzy \
  -y

colcon build \
  --packages-up-to mobile_robot_bringup \
  --symlink-install \
  --event-handlers console_direct+

source install/setup.bash

python3 -m py_compile \
  src/mobile_robot_control/launch/mobile_robot_control.launch.py

echo "Phase1B-Part28 verification completed."
