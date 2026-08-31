#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

echo "== ROS 2 Jazzy =="
source /opt/ros/jazzy/setup.bash
cd "${ROOT}/ros2_ws"

rosdep install \
  --from-paths src \
  --ignore-src \
  --rosdistro jazzy \
  -y

colcon build \
  --symlink-install \
  --event-handlers console_direct+

source install/setup.bash

colcon test \
  --event-handlers console_direct+

colcon test-result --verbose

echo "== ESP32 PlatformIO =="
cd "${ROOT}/firmware/esp32"
pio run

echo "== Documentation =="
cd "${ROOT}"
python3 -m pip install -r requirements-docs.txt
mkdocs build --strict --clean

echo "Local CI completed successfully."
