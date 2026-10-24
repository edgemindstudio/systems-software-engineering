#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{ echo "=== LAB 08-06 VALIDATION ==="; date; echo; echo "=== CLEAN BUILD ==="; make clean; make
echo; echo "=== PROGRAM OUTPUT ==="; output="$(./build/ipv6_demo)"; printf '%s\n' "$output"
grep -q '^ipv6_received=ipv6-ok$' <<<"$output"; grep -q '^ipv6_loopback_verified=yes$' <<<"$output"
echo "ipv6_tcp_verified=yes"; echo; echo "=== RESULT ==="; echo PASS; } > evidence/validation.txt 2>&1
cat evidence/validation.txt
