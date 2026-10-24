#!/usr/bin/env bash
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
labs=(
 "lab-06-01-virtual-address-space"
 "lab-06-02-anonymous-mmap"
 "lab-06-03-memory-protection-guard-page"
 "lab-06-04-fork-copy-on-write-memory"
 "lab-06-05-page-residency-faults"
 "lab-06-06-madvise-reclamation"
 "lab-06-07-address-space-limits"
)
printf '========================================\n'
printf '06 — MEMORY\n'
printf 'SECTION VALIDATION\n'
printf '========================================\n'
for lab in "${labs[@]}"; do
 printf '\n>>> %s\n' "$lab"
 (cd "$ROOT_DIR/$lab" && ./scripts/validate.sh)
done
printf '\n========================================\n'
printf 'ALL MEMORY LABS PASSED\n'
printf '========================================\n'
