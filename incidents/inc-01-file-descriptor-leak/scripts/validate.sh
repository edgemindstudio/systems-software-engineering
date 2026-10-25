#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{ echo "=== INC-01 ==="; date; make clean; make; ./build/incident_01_file_descriptor_leak; } > evidence/validation.txt 2>&1
grep -q '^leak_reproduced=yes$' evidence/validation.txt
grep -q '^cleanup_verified=yes$' evidence/validation.txt
echo PASS >> evidence/validation.txt
cat evidence/validation.txt
