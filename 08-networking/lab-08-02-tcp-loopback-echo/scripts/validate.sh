#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{ echo "=== LAB 08-02 VALIDATION ==="; date; echo; echo "=== CLEAN BUILD ==="; make clean; make
echo; echo "=== PROGRAM OUTPUT ==="; output="$(./build/tcp_echo_demo)"; printf '%s\n' "$output"
grep -q '^server_received=tcp-echo$' <<<"$output"; grep -q '^client_received=tcp-echo$' <<<"$output"; grep -q '^tcp_round_trip_verified=yes$' <<<"$output"
echo "tcp_loopback_verified=yes"; echo; echo "=== RESULT ==="; echo PASS; } > evidence/validation.txt 2>&1
cat evidence/validation.txt
