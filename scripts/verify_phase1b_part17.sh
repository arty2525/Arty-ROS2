#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

cd "${ROOT}/firmware/esp32"

pio run

grep -q "left_motor.set_inverted" src/main.cpp
grep -q "right_motor.set_inverted" src/main.cpp
grep -q "left_encoder.set_inverted" src/main.cpp
grep -q "right_encoder.set_inverted" src/main.cpp

if grep -q "kRightPwmChannel,[[:space:]]*true" src/main.cpp; then
  echo "ERROR: hardcoded right motor inversion remains"
  exit 1
fi

if grep -q "kRightEncoderBPin,[[:space:]]*true" src/main.cpp; then
  echo "ERROR: hardcoded right encoder inversion remains"
  exit 1
fi

grep -q "State::kMagicLow" src/protocol.cpp
grep -q "State::kMagicHigh" src/protocol.cpp

echo "Phase1B-Part17 verification completed."
