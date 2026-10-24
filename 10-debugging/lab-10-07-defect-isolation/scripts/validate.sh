#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 10-07 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== PROGRAM OUTPUT ==="
 output="$(./build/defect_demo)"; printf '%s\n' "$output"
 grep -q '^expected=1$' <<<"$output"
 grep -q '^buggy_result=0$' <<<"$output"
 grep -q '^fixed_result=1$' <<<"$output"
 grep -q '^bug_reproduced=yes$' <<<"$output"
 grep -q '^fix_verified=yes$' <<<"$output"
 echo "defect_isolation_verified=yes"
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
