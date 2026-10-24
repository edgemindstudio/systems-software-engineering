#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 05-04 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== PROGRAM OUTPUT ==="
 output="$(./build/mask_demo)"; printf '%s\n' "$output"
 echo; echo "=== ASSERTIONS ==="
 grep -q '^pending_before_sigsuspend=yes$' <<<"$output"
 grep -q '^handler_observed_after_unblock=yes$' <<<"$output"
 echo "masked_pending_signal_verified=yes"
 echo "sigsuspend_wakeup_verified=yes"
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
