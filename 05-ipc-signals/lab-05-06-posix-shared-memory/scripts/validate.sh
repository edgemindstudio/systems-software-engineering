#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 05-06 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== PROGRAM OUTPUT ==="
 output="$(./build/shm_demo)"; printf '%s\n' "$output"
 echo; echo "=== ASSERTIONS ==="
 grep -q '^shared_ready=1$' <<<"$output"
 grep -q '^shared_value=2026$' <<<"$output"
 grep -q '^shared_message=shared-memory-visible$' <<<"$output"
 grep -q '^shared_memory_unlinked=yes$' <<<"$output"
 echo "shared_memory_visibility_verified=yes"
 echo "shared_memory_cleanup_verified=yes"
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
