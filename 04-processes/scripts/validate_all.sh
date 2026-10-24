#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

labs=(
  "lab-04-01-fork-copy-on-write"
  "lab-04-02-exec-image-replacement"
  "lab-04-03-waitpid-child-reaping"
  "lab-04-04-process-tree-ancestry"
  "lab-04-05-fork-exec-inheritance"
  "lab-04-06-process-groups-sessions"
)

printf '========================================\n'
printf '04 — PROCESSES\n'
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
printf 'ALL PROCESS LABS PASSED\n'
printf '========================================\n'
