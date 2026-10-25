#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"
mkdir -p evidence

{
    echo "========================================"
    echo "THREAD RUNTIME VALIDATION"
    echo "========================================"
    date
    echo

    echo "=== TOOLCHAIN ==="
    clang --version | head -n 1
    uname -srmo
    echo

    echo "=== STRICT BUILD ==="
    make clean
    make
    echo

    echo "=== FUNCTIONAL / STRESS TESTS ==="
    make test
    echo

    echo "=== BENCHMARK / CONSISTENCY CHECK ==="
    make benchmark
    echo

    echo "=== ASAN + UBSAN VALIDATION ==="
    make sanitize
    echo

    echo "=== THREADSANITIZER VALIDATION ==="
    make tsan
    echo

    echo "=== RESTORE NORMAL BUILD ==="
    make clean
    make
    echo

    echo "=== RESULT ==="
    echo "PASS"
} > evidence/validation.txt 2>&1

cat evidence/validation.txt
