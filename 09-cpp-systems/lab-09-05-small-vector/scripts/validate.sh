#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{ echo "=== LAB 09-05 VALIDATION ==="; date; echo; echo "=== CLEAN BUILD ==="; make clean; make
echo; echo "=== PROGRAM OUTPUT ==="; output="$(./build/small_vector_demo)"; printf '%s\n' "$output"
grep -q '^size=10$' <<<"$output"; grep -q '^first=10$' <<<"$output"; grep -q '^last=100$' <<<"$output"; grep -q '^bounds_exception_caught=yes$' <<<"$output"; grep -q '^container_invariants_verified=yes$' <<<"$output"
echo "small_vector_verified=yes"; echo; echo "=== RESULT ==="; echo PASS; } > evidence/validation.txt 2>&1
cat evidence/validation.txt
