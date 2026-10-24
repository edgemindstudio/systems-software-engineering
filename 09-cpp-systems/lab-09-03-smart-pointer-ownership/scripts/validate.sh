#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{ echo "=== LAB 09-03 VALIDATION ==="; date; echo; echo "=== CLEAN BUILD ==="; make clean; make
echo; echo "=== PROGRAM OUTPUT ==="; output="$(./build/smart_pointer_demo)"; printf '%s\n' "$output"
grep -q '^shared_use_count_initial=1$' <<<"$output"; grep -q '^shared_use_count_nested=2$' <<<"$output"; grep -q '^shared_use_count_after_nested=1$' <<<"$output"; grep -q '^weak_expired_after_owner_scope=yes$' <<<"$output"
echo "smart_pointer_lifetime_verified=yes"; echo; echo "=== RESULT ==="; echo PASS; } > evidence/validation.txt 2>&1
cat evidence/validation.txt
