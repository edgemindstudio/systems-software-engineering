#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 07-03 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== PROGRAM OUTPUT ==="
 output="$(./build/condition_demo)"; printf '%s\n' "$output"
 grep -q '^waiter_observed_ready=yes$' <<<"$output"
 grep -q '^predicate_ready=1$' <<<"$output"
 grep -q '^waiter_completed=yes$' <<<"$output"
 echo "condition_predicate_coordination_verified=yes"
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
