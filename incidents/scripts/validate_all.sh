#!/usr/bin/env bash
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
incidents=(
 inc-01-file-descriptor-leak
 inc-02-zombie-child
 inc-03-pipe-deadlock
 inc-04-use-after-free
 inc-05-data-race
 inc-06-tcp-framing-failure
 inc-07-shared-library-lookup
 inc-08-execute-permission
 inc-09-sparse-file-space
 inc-10-tiny-write-regression
)
printf '========================================\n'
printf 'SYSTEMS INCIDENT INVESTIGATIONS\n'
printf 'SECTION VALIDATION\n'
printf '========================================\n'
for incident in "${incidents[@]}"; do
 printf '\n>>> %s\n' "$incident"
 (cd "$ROOT_DIR/$incident" && ./scripts/validate.sh)
done
printf '\n========================================\n'
printf 'ALL INCIDENT INVESTIGATIONS PASSED\n'
printf '========================================\n'
