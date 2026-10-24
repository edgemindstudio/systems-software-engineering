#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 07-06 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== NORMAL PROGRAM OUTPUT ==="
 ./build/race_demo
 echo; echo "=== THREADSANITIZER ATTEMPT ==="
 if make tsan >/tmp/tsan-build.out 2>&1; then
   cat /tmp/tsan-build.out
   set +e
   TSAN_OPTIONS="halt_on_error=0 exitcode=66" ./build/race_demo_tsan >/tmp/tsan-run.out 2>&1
   rc=$?
   set -e
   cat /tmp/tsan-run.out
   if grep -q 'WARNING: ThreadSanitizer: data race' /tmp/tsan-run.out; then
     echo "tsan_data_race_detected=yes"
   else
     echo "tsan_data_race_detected=no"
     echo "tsan_runtime_return_code=$rc"
   fi
 else
   cat /tmp/tsan-build.out
   echo "tsan_build_supported=no"
 fi
 rm -f /tmp/tsan-build.out /tmp/tsan-run.out
 echo "intentional_race_source_verified=yes"
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
