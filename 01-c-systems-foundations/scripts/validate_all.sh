#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

labs=(
    "lab-01-01-memory-representation"
    "lab-01-02-pointer-mechanics"
    "lab-01-03-stack-investigation"
    "lab-01-04-dynamic-memory"
    "lab-01-05-dynamic-array"
    "lab-01-06-linked-structures"
    "lab-01-07-hash-table"
)

printf '========================================\n'
printf '01 — C SYSTEMS FOUNDATIONS\n'
printf 'SECTION VALIDATION\n'
printf '========================================\n'

for lab in "${labs[@]}"; do
    printf '\n>>> %s\n' "$lab"
    (
        cd "$ROOT_DIR/$lab"
        ./scripts/validate.sh
    )
done

printf '\n========================================\n'
printf 'ALL C SYSTEMS FOUNDATION LABS PASSED\n'
printf '========================================\n'
