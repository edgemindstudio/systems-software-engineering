#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 07-07 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== PROGRAM OUTPUT ==="
 output="$(./build/rwlock_demo)"; printf '%s\n' "$output"
 grep -q '^final_value=1000$' <<<"$output"
 grep -q '^expected_final=1000$' <<<"$output"
 grep -q '^rwlock_writer_correct=yes$' <<<"$output"
 echo "rwlock_shared_state_verified=yes"
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
