#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{ echo "=== LAB 08-03 VALIDATION ==="; date; echo; echo "=== CLEAN BUILD ==="; make clean; make
echo; echo "=== PROGRAM OUTPUT ==="; output="$(./build/udp_demo)"; printf '%s\n' "$output"
grep -q '^received=udp-message$' <<<"$output"; grep -q '^peer_family_ipv4=yes$' <<<"$output"; grep -q '^udp_exchange_verified=yes$' <<<"$output"
echo "udp_datagram_verified=yes"; echo; echo "=== RESULT ==="; echo PASS; } > evidence/validation.txt 2>&1
cat evidence/validation.txt
