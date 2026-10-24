#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{ echo "=== LAB 09-06 VALIDATION ==="; date; echo; echo "=== CLEAN BUILD ==="; make clean; make
echo; echo "=== PROGRAM OUTPUT ==="; output="$(./build/thread_safe_queue_demo)"; printf '%s\n' "$output"
grep -q '^expected_sum=5050$' <<<"$output"; grep -q '^actual_sum=5050$' <<<"$output"; grep -q '^consumer_joined=yes$' <<<"$output"; grep -q '^thread_safe_queue_verified=yes$' <<<"$output"
echo "cpp_queue_coordination_verified=yes"; echo; echo "=== RESULT ==="; echo PASS; } > evidence/validation.txt 2>&1
cat evidence/validation.txt
