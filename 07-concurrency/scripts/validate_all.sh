#!/usr/bin/env bash
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
labs=(
 "lab-07-01-thread-lifecycle"
 "lab-07-02-mutex-counter"
 "lab-07-03-condition-variable"
 "lab-07-04-bounded-producer-consumer"
 "lab-07-05-atomic-counter"
 "lab-07-06-race-detection"
 "lab-07-07-reader-writer-lock"
 "lab-07-08-bounded-worker-pool"
)
printf '========================================\n'
printf '07 — CONCURRENCY\n'
printf 'SECTION VALIDATION\n'
printf '========================================\n'
for lab in "${labs[@]}"; do
 printf '\n>>> %s\n' "$lab"
 (cd "$ROOT_DIR/$lab" && ./scripts/validate.sh)
done
printf '\n========================================\n'
printf 'ALL CONCURRENCY LABS PASSED\n'
printf '========================================\n'
