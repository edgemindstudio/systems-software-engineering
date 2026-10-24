#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 05-02 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== PIPELINE OUTPUT ==="
 output="$(./build/pipeline_demo)"; printf '%s\n' "$output"
 echo; echo "=== ASSERTIONS ==="
 grep -Eq '^[[:space:]]*2 alpha$' <<<"$output"
 grep -Eq '^[[:space:]]*1 beta$' <<<"$output"
 echo "pipeline_counts_verified=yes"
 echo; echo "=== STRACE PROCESS/DESCRIPTORS ==="
 if command -v strace >/dev/null 2>&1; then
   strace -f -e trace=process,pipe,pipe2,dup2,close ./build/pipeline_demo 2>&1 | grep -E 'pipe|clone|fork|vfork|dup2|execve|wait4|close' || true
 else
   echo "strace unavailable; trace skipped"
 fi
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
