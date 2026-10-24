#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 07-05 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== PROGRAM OUTPUT ==="
 output="$(./build/atomic_counter_demo)"; printf '%s\n' "$output"
 grep -q '^expected=800000$' <<<"$output"
 grep -q '^actual=800000$' <<<"$output"
 grep -q '^atomic_counter_correct=yes$' <<<"$output"
 echo "atomic_increment_verified=yes"
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
