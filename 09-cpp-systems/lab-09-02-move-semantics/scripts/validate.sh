#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{ echo "=== LAB 09-02 VALIDATION ==="; date; echo; echo "=== CLEAN BUILD ==="; make clean; make
echo; echo "=== PROGRAM OUTPUT ==="; output="$(./build/move_demo)"; printf '%s\n' "$output"
grep -q '^source_empty_after_move=yes$' <<<"$output"; grep -q '^destination_size=4$' <<<"$output"; grep -q '^destination_values=11,22,33,44$' <<<"$output"
echo "move_ownership_transfer_verified=yes"; echo; echo "=== RESULT ==="; echo PASS; } > evidence/validation.txt 2>&1
cat evidence/validation.txt
