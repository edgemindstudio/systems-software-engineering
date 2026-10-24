#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{ echo "=== LAB 09-04 VALIDATION ==="; date; echo; echo "=== CLEAN BUILD ==="; make clean; make
echo; echo "=== PROGRAM OUTPUT ==="; output="$(./build/system_error_demo)"; printf '%s\n' "$output"
grep -q '^good_open_fd_nonnegative=yes$' <<<"$output"; grep -q '^bad_open_caught_system_error=yes$' <<<"$output"; grep -q '^exception_path_survived=yes$' <<<"$output"
echo "system_error_path_verified=yes"; echo; echo "=== RESULT ==="; echo PASS; } > evidence/validation.txt 2>&1
cat evidence/validation.txt
