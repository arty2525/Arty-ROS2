#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
CONFIG="${ROOT}/ros2_ws/src/mobile_robot_bringup/config/robot.yaml"
PART16="${ROOT}/scripts/verify_phase1b_part16.sh"
VALIDATOR="${ROOT}/scripts/validate_robot_config.py"

python3 -m py_compile "${VALIDATOR}"
bash -n "${PART16}"

python3 "${VALIDATOR}" "${CONFIG}"

if python3 "${VALIDATOR}" "${CONFIG}" --require-complete; then
  echo "ERROR: production config ถูกมองว่าครบ ทั้งที่ยังมีค่ากายภาพที่ยังไม่ยืนยัน" >&2
  exit 1
fi

if grep -Eq \
  '(ticks_per_revolution|wheel_separation|wheel_width|base_length|base_width|base_height|caster_ball_radius|front_caster_offset|rear_caster_offset):=[0-9]' \
  "${PART16}"; then
  echo "ERROR: Part16 verifier มีการฝังค่า physical parameter โดยตรง" >&2
  exit 1
fi

grep -q 'validate_robot_config.py' "${PART16}"
grep -q 'config\["geometry"\]\["wheel_separation"\]' "${PART16}"
grep -q 'config\["encoder"\]\["ticks_per_revolution"\]' "${PART16}"

echo "Phase1B-Part25 verification completed."
