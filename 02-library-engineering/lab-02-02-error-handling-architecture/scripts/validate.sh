#!/usr/bin/env bash
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"
mkdir -p evidence
{
    echo "=== LAB 02-02 VALIDATION ==="
    date
    echo
    echo "=== CLEAN BUILD ==="
    make clean
    make
    echo
    echo "=== PROGRAM OUTPUT ==="
    ./build/error_demo
    echo
    echo "=== TESTS ==="
    make test
    echo
    echo "=== RESULT ==="
    echo "PASS"
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
