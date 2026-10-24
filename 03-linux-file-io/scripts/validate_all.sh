#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

labs=(
  "lab-03-01-file-descriptors-robust-io"
  "lab-03-02-file-offsets-duplication"
  "lab-03-03-file-metadata-links"
  "lab-03-04-directory-traversal"
  "lab-03-05-sparse-files"
  "lab-03-06-vectored-io"
  "lab-03-07-memory-mapped-file-io"
  "lab-03-08-advisory-file-locking"
)

printf '========================================\n'
printf '03 — LINUX FILE I/O\n'
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
printf 'ALL LINUX FILE I/O LABS PASSED\n'
printf '========================================\n'
