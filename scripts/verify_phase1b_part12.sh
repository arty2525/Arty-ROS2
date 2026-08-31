#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

cd "${ROOT}/firmware/esp32"
pio run

echo "Phase1B-Part12 firmware build completed."
