#!/usr/bin/env bash
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
labs=(
 "lab-10-01-gdb-segfault-analysis"
 "lab-10-02-lldb-crash-analysis"
 "lab-10-03-valgrind-heap-diagnostics"
 "lab-10-04-address-sanitizer"
 "lab-10-05-undefined-behavior-sanitizer"
 "lab-10-06-assertions-invariants"
 "lab-10-07-defect-isolation"
)
printf '========================================\n'
printf '10 — DEBUGGING\n'
printf 'SECTION VALIDATION\n'
printf '========================================\n'
for lab in "${labs[@]}"; do
 printf '\n>>> %s\n' "$lab"
 (cd "$ROOT_DIR/$lab" && ./scripts/validate.sh)
done
printf '\n========================================\n'
printf 'ALL DEBUGGING LABS PASSED\n'
printf '========================================\n'
