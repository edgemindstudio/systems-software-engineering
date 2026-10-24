#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 05-01 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== PROGRAM OUTPUT ==="; output="$(./build/pipe_demo)"; printf '%s\n' "$output"
 echo; echo "=== ASSERTIONS ==="
 grep -q '^child_received=hello-through-pipe$' <<<"$output"
 grep -q '^eof_after_writer_close=yes$' <<<"$output"
 echo "pipe_delivery_verified=yes"
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
