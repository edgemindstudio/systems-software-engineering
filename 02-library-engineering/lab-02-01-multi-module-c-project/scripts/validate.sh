#!/usr/bin/env bash
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"
mkdir -p evidence
{
    echo "=== LAB 02-01 VALIDATION ==="
    date
    echo
    echo "=== CLEAN BUILD ==="
    make clean
    make
    echo
    echo "=== PROGRAM OUTPUT ==="
    ./build/telemetry_demo
    echo
    echo "=== TESTS ==="
    make test
    echo
    echo "=== PUBLIC HEADER ==="
    find include -maxdepth 1 -type f -printf '%f\n' | sort
    echo
    echo "=== PRIVATE HEADER ==="
    find src -maxdepth 1 -name '*internal*.h' -printf '%f\n' | sort
    echo
    echo "=== RESULT ==="
    echo "PASS"
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
