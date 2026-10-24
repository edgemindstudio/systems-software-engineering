#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 11-06 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== O0 OUTPUT ==="; o0="$(./build/optimization_o0)"; printf '%s\n' "$o0"
 echo; echo "=== O2 OUTPUT ==="; o2="$(./build/optimization_o2)"; printf '%s\n' "$o2"
 r0="$(awk -F= '/^result=/{print $2}' <<<"$o0")"
 r2="$(awk -F= '/^result=/{print $2}' <<<"$o2")"
 t0="$(awk -F= '/^elapsed_seconds=/{print $2}' <<<"$o0")"
 t2="$(awk -F= '/^elapsed_seconds=/{print $2}' <<<"$o2")"
 test "$r0" = "$r2"
 awk -v a="$t0" -v b="$t2" 'BEGIN { exit !(a > 0 && b > 0) }'
 echo "results_equal=yes"
 awk -v a="$t0" -v b="$t2" 'BEGIN { printf "o0_to_o2_ratio=%.3f\n", a / b }'
 echo "optimization_comparison_verified=yes"
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
