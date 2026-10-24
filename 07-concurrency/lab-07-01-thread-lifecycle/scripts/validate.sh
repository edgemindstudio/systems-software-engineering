#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 07-01 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== PROGRAM OUTPUT ==="
 output="$(./build/thread_lifecycle_demo)"; printf '%s\n' "$output"
 grep -q '^thread=0 input=2 result=4$' <<<"$output"
 grep -q '^thread=3 input=5 result=25$' <<<"$output"
 grep -q '^all_threads_joined=yes$' <<<"$output"
 echo "thread_lifecycle_verified=yes"
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
