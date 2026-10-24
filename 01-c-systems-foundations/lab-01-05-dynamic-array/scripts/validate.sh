#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"

mkdir -p evidence

{
    echo "=== VALIDATION ==="
    date
    echo
    echo "=== COMPILER ==="
    "${CC:-clang}" --version | head -n 1 || true

    echo
    echo "=== CLEAN BUILD ==="
    make clean
    make

    echo
    echo "=== PROGRAM OUTPUT ==="
    ./build/lab

    echo
    echo "=== TESTS ==="
    make test

    echo
    echo "=== SANITIZERS ==="
    make sanitize

    echo
    echo "=== RESULT ==="
    echo "PASS"
} > evidence/validation.txt 2>&1

cat evidence/validation.txt
