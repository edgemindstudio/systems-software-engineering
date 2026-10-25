#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 12-04 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== PROGRAM OUTPUT ==="; ./build/dynamic_deps_demo
 echo; echo "=== INTERPRETER ==="
 readelf -l ./build/dynamic_deps_demo | grep 'Requesting program interpreter'
 echo; echo "=== NEEDED ENTRIES ==="
 readelf -d ./build/dynamic_deps_demo | grep 'NEEDED'
 echo; echo "=== LDD ==="
 ldd ./build/dynamic_deps_demo
 readelf -d ./build/dynamic_deps_demo | grep -q 'NEEDED'
 ldd ./build/dynamic_deps_demo | grep -q 'libc'
 echo "dynamic_dependencies_verified=yes"
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
