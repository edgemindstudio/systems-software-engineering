#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 07-08 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== PROGRAM OUTPUT ==="
 output="$(./build/worker_pool_demo)"; printf '%s\n' "$output"
 grep -q '^tasks_submitted=40$' <<<"$output"
 grep -q '^tasks_completed=40$' <<<"$output"
 grep -q '^expected_sum=22140$' <<<"$output"
 grep -q '^actual_sum=22140$' <<<"$output"
 grep -q '^worker_pool_correct=yes$' <<<"$output"
 echo "worker_pool_shutdown_verified=yes"
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
