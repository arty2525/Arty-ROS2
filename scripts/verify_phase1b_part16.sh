#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
CONFIG="${ROOT}/ros2_ws/src/mobile_robot_bringup/config/robot.yaml"
URDF_OUT="$(mktemp)"
PARAMS_OUT="$(mktemp)"

cleanup() {
  rm -f "${URDF_OUT}" "${PARAMS_OUT}"
}
trap cleanup EXIT

source /opt/ros/jazzy/setup.bash

python3 "${ROOT}/scripts/validate_robot_config.py" "${CONFIG}"

cd "${ROOT}/ros2_ws"

rosdep install \
  --from-paths src \
  --ignore-src \
  --rosdistro jazzy \
  -y

colcon build \
  --packages-up-to \
    mobile_robot_description \
    mobile_robot_control \
    mobile_robot_bringup \
  --symlink-install \
  --event-handlers console_direct+

source install/setup.bash

python3 - "${CONFIG}" > "${PARAMS_OUT}" <<'PY'
from pathlib import Path
import sys
import yaml

path = Path(sys.argv[1])
with path.open("r", encoding="utf-8") as stream:
    config = yaml.safe_load(stream)

values = {
    "serial_device": config["serial_device"],
    "baud_rate": config["baud_rate"],
    "telemetry_timeout_ms": config["communication"]["telemetry_timeout_ms"],
    "ticks_per_revolution": config["encoder"]["ticks_per_revolution"],
    "left_motor_inverted": str(config["inversion"]["left_motor"]).lower(),
    "right_motor_inverted": str(config["inversion"]["right_motor"]).lower(),
    "left_encoder_inverted": str(config["inversion"]["left_encoder"]).lower(),
    "right_encoder_inverted": str(config["inversion"]["right_encoder"]).lower(),
    "wheel_radius": config["geometry"]["wheel_radius"],
    "wheel_separation": config["geometry"]["wheel_separation"],
    "wheel_width": config["geometry"]["wheel_width"],
    "base_length": config["geometry"]["base_length"],
    "base_width": config["geometry"]["base_width"],
    "base_height": config["geometry"]["base_height"],
    "caster_ball_radius": config["geometry"]["caster_ball_radius"],
    "front_caster_offset": config["geometry"]["front_caster_offset"],
    "rear_caster_offset": config["geometry"]["rear_caster_offset"],
}

for key, value in values.items():
    print(f"{key}={value}")
PY

declare -a XACRO_ARGS=()
while IFS='=' read -r key value; do
  XACRO_ARGS+=("${key}:=${value}")
done < "${PARAMS_OUT}"

xacro \
  "$(ros2 pkg prefix mobile_robot_description)/share/mobile_robot_description/urdf/mobile_robot.urdf.xacro" \
  "${XACRO_ARGS[@]}" \
  > "${URDF_OUT}"

grep -q 'name="left_wheel_joint"' "${URDF_OUT}"
grep -q 'name="right_wheel_joint"' "${URDF_OUT}"
grep -q 'name="front_caster_ball_joint"' "${URDF_OUT}"
grep -q 'name="rear_caster_ball_joint"' "${URDF_OUT}"
grep -q 'radius="0.0325"' "${URDF_OUT}"

echo "Phase1B-Part16 verification completed without fabricated physical parameters."
