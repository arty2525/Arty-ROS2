#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

cd "${ROOT}"

test -f requirements-platformio.txt
grep -qx "platformio==6.1.18" requirements-platformio.txt

grep -q \
  "cache-dependency-path: requirements-platformio.txt" \
  .github/workflows/ci.yml

grep -q \
  "cache-dependency-path: requirements-docs.txt" \
  .github/workflows/ci.yml

grep -q \
  "python -m pip install -r requirements-platformio.txt" \
  .github/workflows/ci.yml

echo "Phase1B-Part22 CI configuration verification completed."
