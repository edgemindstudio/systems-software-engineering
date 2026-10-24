#!/usr/bin/env bash
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
labs=(
 "lab-05-01-anonymous-pipes"
 "lab-05-02-multi-process-pipeline"
 "lab-05-03-safe-signal-handling"
 "lab-05-04-signal-masks-sigsuspend"
 "lab-05-05-named-fifo"
 "lab-05-06-posix-shared-memory"
)
printf '========================================\n'
printf '05 — IPC & SIGNALS\n'
printf 'SECTION VALIDATION\n'
printf '========================================\n'
for lab in "${labs[@]}"; do
 printf '\n>>> %s\n' "$lab"
 (cd "$ROOT_DIR/$lab" && ./scripts/validate.sh)
done
printf '\n========================================\n'
printf 'ALL IPC & SIGNAL LABS PASSED\n'
printf '========================================\n'
