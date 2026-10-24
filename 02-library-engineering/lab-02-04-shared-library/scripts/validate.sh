#!/usr/bin/env bash
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"
mkdir -p evidence
{
    echo "=== LAB 02-04 VALIDATION ==="
    date
    echo
    echo "=== CLEAN BUILD ==="
    make clean
    make
    echo
    echo "=== SYMLINKS ==="
    ls -l build/lib
    echo
    echo "=== PROGRAM OUTPUT ==="
    ./build/bin/shared_demo
    echo
    echo "=== TESTS ==="
    make test
    echo
    echo "=== SONAME ==="
    readelf -d build/lib/libcounter.so.1.0.0 | grep SONAME
    echo
    echo "=== RUNPATH ==="
    readelf -d build/bin/shared_demo | grep -E 'RPATH|RUNPATH'
    echo
    echo "=== EXPORTED SYMBOLS ==="
    nm -D --defined-only build/lib/libcounter.so.1.0.0
    echo
    echo "=== RUNTIME DEPENDENCIES ==="
    ldd build/bin/shared_demo
    echo
    echo "=== RESULT ==="
    echo "PASS"
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
