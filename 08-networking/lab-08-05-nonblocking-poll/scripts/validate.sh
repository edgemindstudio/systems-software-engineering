#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{ echo "=== LAB 08-05 VALIDATION ==="; date; echo; echo "=== CLEAN BUILD ==="; make clean; make
echo; echo "=== PROGRAM OUTPUT ==="; output="$(./build/poll_demo)"; printf '%s\n' "$output"
grep -q '^initial_recv_eagain=yes$' <<<"$output"; grep -q '^poll_readable=yes$' <<<"$output"; grep -q '^nonblocking_poll_verified=yes$' <<<"$output"
echo "nonblocking_readiness_verified=yes"; echo; echo "=== RESULT ==="; echo PASS; } > evidence/validation.txt 2>&1
cat evidence/validation.txt
