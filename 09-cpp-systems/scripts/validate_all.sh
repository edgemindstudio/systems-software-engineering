#!/usr/bin/env bash
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
labs=(
 "lab-09-01-raii-file-descriptor"
 "lab-09-02-move-semantics"
 "lab-09-03-smart-pointer-ownership"
 "lab-09-04-system-error-handling"
 "lab-09-05-small-vector"
 "lab-09-06-thread-safe-queue"
)
printf '========================================\n'
printf '09 — C++ SYSTEMS\n'
printf 'SECTION VALIDATION\n'
printf '========================================\n'
for lab in "${labs[@]}"; do
 printf '\n>>> %s\n' "$lab"
 (cd "$ROOT_DIR/$lab" && ./scripts/validate.sh)
done
printf '\n========================================\n'
printf 'ALL C++ SYSTEMS LABS PASSED\n'
printf '========================================\n'
