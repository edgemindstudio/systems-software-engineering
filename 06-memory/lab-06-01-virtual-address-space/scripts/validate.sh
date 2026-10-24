#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 06-01 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== PROGRAM OUTPUT ==="
 ./build/address_space_demo
 echo; echo "=== PROC MAPS SNAPSHOT ==="
 ./build/address_space_demo >/tmp/address-space-demo.out &
 pid=$!
 wait "$pid"
 cat /tmp/address-space-demo.out
 grep -E '\[heap\]|\[stack\]|r-xp' /proc/$$/maps | head -n 12
 rm -f /tmp/address-space-demo.out
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
