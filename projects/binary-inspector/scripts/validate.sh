#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"
mkdir -p evidence

{
    echo "========================================"
    echo "BINARY INSPECTOR VALIDATION"
    echo "========================================"
    date
    echo

    echo "=== TOOLCHAIN ==="
    clang --version | head -n 1
    readelf --version | head -n 1
    uname -srmo
    echo

    echo "=== STRICT BUILD ==="
    make clean
    make
    echo

    echo "=== INTEGRATION / ORACLE TESTS ==="
    make test
    echo

    echo "=== REPRESENTATIVE INSPECTION ==="
    clang -std=c17 -g -O0 tests/fixture.c -o workspace/fixture-demo
    ./build/binspect workspace/fixture-demo summary
    echo "-- sections (selected) --"
    ./build/binspect workspace/fixture-demo sections | grep -E $'\t(\.text|\.data|\.symtab|\.dynsym)\t'
    echo "-- needed --"
    ./build/binspect workspace/fixture-demo needed
    echo

    echo "=== ASAN + UBSAN VALIDATION ==="
    make sanitize
    echo

    echo "=== RESTORE NORMAL BUILD ==="
    make clean
    make
    echo

    echo "=== RESULT ==="
    echo "PASS"
} > evidence/validation.txt 2>&1

cat evidence/validation.txt
