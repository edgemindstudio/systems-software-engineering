#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{ echo "=== LAB 08-04 VALIDATION ==="; date; echo; echo "=== CLEAN BUILD ==="; make clean; make
echo; echo "=== PROGRAM OUTPUT ==="; output="$(./build/framing_demo)"; printf '%s\n' "$output"
grep -q '^decoded_length=21$' <<<"$output"; grep -q '^decoded_message=framed-message-works!$' <<<"$output"; grep -q '^framing_verified=yes$' <<<"$output"
echo "length_prefix_protocol_verified=yes"; echo; echo "=== RESULT ==="; echo PASS; } > evidence/validation.txt 2>&1
cat evidence/validation.txt
