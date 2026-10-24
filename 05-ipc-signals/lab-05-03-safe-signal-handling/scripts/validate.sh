#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 05-03 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== PROGRAM OUTPUT ==="
 output="$(./build/signal_demo)"; printf '%s\n' "$output"
 echo; echo "=== ASSERTIONS ==="
 grep -q '^handler_received_SIGUSR1$' <<<"$output"
 grep -q '^main_observed_signal=yes$' <<<"$output"
 echo "sigaction_delivery_verified=yes"
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
