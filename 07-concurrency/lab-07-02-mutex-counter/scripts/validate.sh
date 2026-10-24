#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 07-02 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== PROGRAM OUTPUT ==="
 output="$(./build/mutex_counter_demo)"; printf '%s\n' "$output"
 grep -q '^expected=400000$' <<<"$output"
 grep -q '^actual=400000$' <<<"$output"
 grep -q '^mutex_counter_correct=yes$' <<<"$output"
 echo "mutex_serialization_verified=yes"
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
