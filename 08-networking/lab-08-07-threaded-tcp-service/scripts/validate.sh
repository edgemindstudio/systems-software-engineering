#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{ echo "=== LAB 08-07 VALIDATION ==="; date; echo; echo "=== CLEAN BUILD ==="; make clean; make
echo; echo "=== PROGRAM OUTPUT ==="; output="$(./build/threaded_service_demo)"; printf '%s\n' "$output"
grep -q '^clients_handled=4$' <<<"$output"; grep -q '^threaded_service_verified=yes$' <<<"$output"
test "$(grep -c '^worker=' <<<"$output")" -eq 4
echo "concurrent_clients_verified=yes"; echo; echo "=== RESULT ==="; echo PASS; } > evidence/validation.txt 2>&1
cat evidence/validation.txt
