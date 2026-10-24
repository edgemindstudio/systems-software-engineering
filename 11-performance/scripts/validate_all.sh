#!/usr/bin/env bash
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
labs=(
 "lab-11-01-monotonic-timing"
 "lab-11-02-repeated-benchmark-statistics"
 "lab-11-03-cache-locality"
 "lab-11-04-syscall-amortization"
 "lab-11-05-allocation-reuse"
 "lab-11-06-optimization-effects"
)
printf '========================================\n'
printf '11 — PERFORMANCE\n'
printf 'SECTION VALIDATION\n'
printf '========================================\n'
for lab in "${labs[@]}"; do
 printf '\n>>> %s\n' "$lab"
 (cd "$ROOT_DIR/$lab" && ./scripts/validate.sh)
done
printf '\n========================================\n'
printf 'ALL PERFORMANCE LABS PASSED\n'
printf '========================================\n'
