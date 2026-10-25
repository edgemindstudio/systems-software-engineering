#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{ echo "=== INC-10 ==="; date; make clean; make; ./build/incident_10_tiny_write_regression; } > evidence/validation.txt 2>&1
grep -q '^regression_reproduced=yes$' evidence/validation.txt
echo PASS >> evidence/validation.txt
cat evidence/validation.txt
