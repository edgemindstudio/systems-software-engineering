#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 07-04 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== PROGRAM OUTPUT ==="
 output="$(./build/producer_consumer_demo)"; printf '%s\n' "$output"
 grep -q '^expected_sum=5050$' <<<"$output"
 grep -q '^actual_sum=5050$' <<<"$output"
 grep -q '^queue_empty=yes$' <<<"$output"
 grep -q '^producer_consumer_correct=yes$' <<<"$output"
 echo "bounded_queue_coordination_verified=yes"
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
