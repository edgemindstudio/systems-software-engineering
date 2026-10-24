#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{ echo "=== LAB 08-01 VALIDATION ==="; date; echo; echo "=== CLEAN BUILD ==="; make clean; make
echo; echo "=== PROGRAM OUTPUT ==="; output="$(./build/address_resolution_demo)"; printf '%s\n' "$output"
grep -q '^resolution_success=yes$' <<<"$output"; count="$(sed -n 's/^resolved_entries=//p' <<<"$output")"; test "$count" -gt 0
echo "address_resolution_verified=yes"; echo; echo "=== RESULT ==="; echo PASS; } > evidence/validation.txt 2>&1
cat evidence/validation.txt
