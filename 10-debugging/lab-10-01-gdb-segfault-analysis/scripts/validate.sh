#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 10-01 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== GDB BATCH ANALYSIS ==="
 set +e
 gdb -q -batch -ex run -ex "bt" ./build/gdb_crash_demo > /tmp/gdb.out 2>&1
 rc=$?
 set -e
 cat /tmp/gdb.out
 grep -q 'Program received signal SIGSEGV' /tmp/gdb.out
 grep -q 'crash_here' /tmp/gdb.out
 grep -q '#0' /tmp/gdb.out
 echo "gdb_return_code=$rc"
 echo "gdb_segfault_backtrace_verified=yes"
 rm -f /tmp/gdb.out
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
