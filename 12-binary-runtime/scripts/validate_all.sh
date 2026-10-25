#!/usr/bin/env bash
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
labs=(
 "lab-12-01-elf-header-inspection"
 "lab-12-02-sections-and-symbols"
 "lab-12-03-relocations"
 "lab-12-04-dynamic-dependencies"
 "lab-12-05-shared-library-runtime"
)
printf '========================================\n'
printf '12 — BINARY & RUNTIME\n'
printf 'SECTION VALIDATION\n'
printf '========================================\n'
for lab in "${labs[@]}"; do
 printf '\n>>> %s\n' "$lab"
 (cd "$ROOT_DIR/$lab" && ./scripts/validate.sh)
done
printf '\n========================================\n'
printf 'ALL BINARY & RUNTIME LABS PASSED\n'
printf '========================================\n'
