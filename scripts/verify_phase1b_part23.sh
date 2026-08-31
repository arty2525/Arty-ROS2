#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

cd "${ROOT}"

test -f mkdocs.yml
grep -q "^site_name: Arty-ROS2$" mkdocs.yml
grep -q "^docs_dir: docs$" mkdocs.yml

grep -q "build_unflags" firmware/esp32/platformio.ini
grep -q -- "-std=gnu++11" firmware/esp32/platformio.ini
grep -q -- "-std=gnu++17" firmware/esp32/platformio.ini

if command -v pio >/dev/null 2>&1; then
  (
    cd firmware/esp32
    pio run
  )
fi

if command -v mkdocs >/dev/null 2>&1; then
  mkdocs build --strict --clean
fi

echo "Phase1B-Part23 verification completed."
