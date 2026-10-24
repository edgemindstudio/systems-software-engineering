#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 06-04 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== PROGRAM OUTPUT ==="
 output="$(./build/cow_demo)"; printf '%s\n' "$output"
 grep -q '^child_value=200$' <<<"$output"
 grep -q '^parent_value_after_child_write=100$' <<<"$output"
 grep -q '^parent_value_after_parent_write=300$' <<<"$output"
 grep -q '^child_exit_success=yes$' <<<"$output"
 echo "post_fork_memory_isolation_verified=yes"
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
