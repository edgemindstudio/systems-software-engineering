#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 12-05 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== LIBRARY FILES ==="; ls -l build/lib
 echo; echo "=== SONAME ==="
 readelf -d build/lib/libruntimemath.so.1.0.0 | grep 'SONAME'
 echo; echo "=== APP RUNPATH / NEEDED ==="
 readelf -d build/bin/runtime_loader_demo | grep -E 'RUNPATH|NEEDED'
 echo; echo "=== LDD ==="
 ldd build/bin/runtime_loader_demo
 echo; echo "=== PROGRAM OUTPUT ==="
 output="$(build/bin/runtime_loader_demo)"; printf '%s\n' "$output"
 echo; echo "=== LOADER TRACE (filtered) ==="
 LD_DEBUG=libs build/bin/runtime_loader_demo 2>&1 | grep -E 'libruntimemath|search path|calling init' | head -n 40 || true
 grep -q '^runtime_multiply=42$' <<<"$output"
 readelf -d build/lib/libruntimemath.so.1.0.0 | grep -q 'libruntimemath.so.1'
 readelf -d build/bin/runtime_loader_demo | grep -q '\$ORIGIN/../lib'
 ldd build/bin/runtime_loader_demo | grep -q 'libruntimemath.so.1'
 echo "shared_library_runtime_verified=yes"
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
