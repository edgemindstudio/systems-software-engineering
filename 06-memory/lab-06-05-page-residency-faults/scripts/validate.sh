#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 06-05 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== PROGRAM OUTPUT ==="
 output="$(./build/residency_demo)"; printf '%s\n' "$output"
 before="$(sed -n 's/^resident_before=//p' <<<"$output")"
 after="$(sed -n 's/^resident_after=//p' <<<"$output")"
 faults="$(sed -n 's/^minor_fault_delta=//p' <<<"$output")"
 test "$after" -eq 64
 test "$after" -ge "$before"
 test "$faults" -gt 0
 echo "all_pages_resident_after_touch=yes"
 echo "minor_faults_observed=yes"
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
