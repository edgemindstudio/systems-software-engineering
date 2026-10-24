#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 10-05 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== UNDEFINEDBEHAVIORSANITIZER ==="
 set +e
 ./build/ubsan_demo > /tmp/ubsan.out 2>&1
 rc=$?
 set -e
 cat /tmp/ubsan.out
 grep -q 'runtime error: signed integer overflow' /tmp/ubsan.out
 test "$rc" -ne 0
 echo "ubsan_return_code=$rc"
 echo "ubsan_signed_overflow_verified=yes"
 rm -f /tmp/ubsan.out
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
