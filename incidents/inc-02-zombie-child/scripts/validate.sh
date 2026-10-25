#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{ echo "=== INC-02 ==="; date; make clean; make; ./build/incident_02_zombie_child; } > evidence/validation.txt 2>&1
grep -q '^zombie_observed=yes$' evidence/validation.txt
grep -q '^child_reaped=yes$' evidence/validation.txt
echo PASS >> evidence/validation.txt
cat evidence/validation.txt
