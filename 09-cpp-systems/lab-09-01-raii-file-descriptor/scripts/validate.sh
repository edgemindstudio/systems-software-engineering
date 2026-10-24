#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{ echo "=== LAB 09-01 VALIDATION ==="; date; echo; echo "=== CLEAN BUILD ==="; make clean; make
echo; echo "=== PROGRAM OUTPUT ==="; output="$(./build/raii_fd_demo)"; printf '%s\n' "$output"
grep -q '^fd_valid_inside_scope=yes$' <<<"$output"; grep -q '^raw_fd_closed_after_scope=yes$' <<<"$output"
echo "raii_fd_cleanup_verified=yes"; echo; echo "=== RESULT ==="; echo PASS; } > evidence/validation.txt 2>&1
cat evidence/validation.txt
