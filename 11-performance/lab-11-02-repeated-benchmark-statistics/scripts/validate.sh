#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 11-02 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== PROGRAM OUTPUT ==="; output="$(./build/stats_demo)"; printf '%s\n' "$output"
 test "$(grep -c '^trial_[0-9]_seconds=' <<<"$output")" -eq 7
 grep -q '^statistics_order_valid=yes$' <<<"$output"
 echo "repeated_statistics_verified=yes"
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
