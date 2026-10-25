#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"
mkdir -p evidence

{
    echo "========================================"
    echo "SYSTEMS UTILITY SUITE VALIDATION"
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

    echo "=== INTEGRATION TESTS ==="
    make test
    echo

    echo "=== REPRESENTATIVE STAT ==="
    ./build/sysutil stat README.md
    echo

    echo "=== REPRESENTATIVE HEX ==="
    ./build/sysutil hex README.md 32
    echo

    echo "=== REPRESENTATIVE PROC ==="
    ./build/sysutil proc self
    echo

    echo "=== SANITIZER VALIDATION ==="
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
