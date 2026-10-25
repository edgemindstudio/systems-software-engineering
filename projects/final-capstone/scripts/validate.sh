#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"
mkdir -p evidence

{
    echo "========================================"
    echo "FINAL CAPSTONE VALIDATION"
    echo "========================================"
    date
    echo

    echo "=== TOOLCHAIN ==="
    clang --version | head -n 1
    python3 --version
    uname -srmo
    echo

    echo "=== STRICT BUILD ==="
    make clean
    make
    echo

    echo "=== INTEGRATION / CONCURRENCY TESTS ==="
    make test
    echo

    echo "=== REPRESENTATIVE DAEMON / CLI ==="
    rm -rf workspace
    mkdir -p workspace
    clang -std=c17 -g -O0 tests/fixture.c -o workspace/fixture-demo

    ./build/sysdiag-daemon \
        --socket workspace/sysdiag-demo.sock \
        --workers 3 \
        --queue 8 \
        > workspace/daemon-demo.log 2>&1 &
    daemon_pid=$!

    for _ in $(seq 1 100); do
        if [[ -S workspace/sysdiag-demo.sock ]]; then
            break
        fi
        sleep 0.02
    done

    ./build/sysdiag --socket workspace/sysdiag-demo.sock PING
    ./build/sysdiag --socket workspace/sysdiag-demo.sock PROC self
    ./build/sysdiag --socket workspace/sysdiag-demo.sock ELF \
        "$(pwd)/workspace/fixture-demo"

    kill -TERM "$daemon_pid"
    wait "$daemon_pid"
    cat workspace/daemon-demo.log
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
