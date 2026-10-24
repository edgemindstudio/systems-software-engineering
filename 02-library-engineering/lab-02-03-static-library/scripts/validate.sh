#!/usr/bin/env bash
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"
mkdir -p evidence
{
    echo "=== LAB 02-03 VALIDATION ==="
    date
    echo
    echo "=== CLEAN BUILD ==="
    make clean
    make
    echo
    echo "=== PROGRAM OUTPUT ==="
    ./build/bin/static_demo
    echo
    echo "=== TESTS ==="
    make test
    echo
    echo "=== ARCHIVE MEMBERS ==="
    ar t build/lib/libtextops.a
    echo
    echo "=== ARCHIVE SYMBOLS ==="
    nm --defined-only build/lib/libtextops.a
    echo
    echo "=== EXECUTABLE TYPE ==="
    file build/bin/static_demo
    echo
    echo "=== RUNTIME DEPENDENCIES ==="
    ldd build/bin/static_demo
    echo
    echo "=== RESULT ==="
    echo "PASS"
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
