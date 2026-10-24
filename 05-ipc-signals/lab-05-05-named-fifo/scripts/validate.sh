#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"; rm -rf workspace; mkdir -p workspace evidence
{
 echo "=== LAB 05-05 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== PROGRAM OUTPUT ==="
 output="$(./build/fifo_demo workspace/channel.fifo)"; printf '%s\n' "$output"
 echo; echo "=== ASSERTIONS ==="
 grep -q '^reader_received=fifo-message$' <<<"$output"
 grep -q '^fifo_type_verified=yes$' <<<"$output"
 test ! -e workspace/channel.fifo
 echo "fifo_delivery_and_cleanup_verified=yes"
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
