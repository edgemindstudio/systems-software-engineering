#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 10-04 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== ADDRESSSANITIZER ==="
 set +e
 ASAN_OPTIONS="halt_on_error=1:detect_leaks=0" ./build/asan_demo > /tmp/asan.out 2>&1
 rc=$?
 set -e
 cat /tmp/asan.out
 grep -q 'AddressSanitizer: heap-buffer-overflow' /tmp/asan.out
 test "$rc" -ne 0
 echo "asan_return_code=$rc"
 echo "asan_heap_overflow_verified=yes"
 rm -f /tmp/asan.out
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
