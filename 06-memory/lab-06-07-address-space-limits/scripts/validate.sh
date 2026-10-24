#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 06-07 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== PROGRAM OUTPUT ==="
 output="$(./build/rlimit_demo)"; printf '%s\n' "$output"
 grep -q '^allocation_failed_under_limit=yes$' <<<"$output"
 grep -q '^child_exit_status=0$' <<<"$output"
 grep -q '^controlled_failure_verified=yes$' <<<"$output"
 echo "rlimit_allocation_failure_verified=yes"
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
