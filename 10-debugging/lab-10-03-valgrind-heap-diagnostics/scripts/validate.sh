#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 10-03 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== VALGRIND MEMCHECK ==="
 set +e
 valgrind --tool=memcheck --error-exitcode=66 ./build/valgrind_demo > /tmp/valgrind.out 2>&1
 rc=$?
 set -e
 cat /tmp/valgrind.out
 grep -q 'Invalid read' /tmp/valgrind.out
 test "$rc" -eq 66
 echo "valgrind_error_exit_code=$rc"
 echo "valgrind_invalid_read_verified=yes"
 rm -f /tmp/valgrind.out
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
