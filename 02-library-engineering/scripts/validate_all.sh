#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

labs=(
    "lab-02-01-multi-module-c-project"
    "lab-02-02-error-handling-architecture"
    "lab-02-03-static-library"
    "lab-02-04-shared-library"
    "lab-02-05-abi-breakage-investigation"
)

printf '========================================\n'
printf '02 — LIBRARY ENGINEERING\n'
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
printf 'ALL LIBRARY ENGINEERING LABS PASSED\n'
printf '========================================\n'
