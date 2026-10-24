#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 06-02 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== PROGRAM OUTPUT ==="
 output="$(./build/anonymous_mmap_demo)"; printf '%s\n' "$output"
 grep -q '^message=anonymous-mapping-ok$' <<<"$output"
 grep -q '^munmap_success=yes$' <<<"$output"
 echo "anonymous_mapping_lifecycle_verified=yes"
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
