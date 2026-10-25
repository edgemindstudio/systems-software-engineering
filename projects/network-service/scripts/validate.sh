#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"
mkdir -p evidence

{
    echo "========================================"
    echo "NETWORK SERVICE VALIDATION"
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

    echo "=== REPRESENTATIVE SERVER/CLIENT ==="
    rm -rf workspace
    mkdir -p workspace
    ./build/netservice-server \
        --port 0 \
        --workers 2 \
        --queue 4 \
        --port-file workspace/port.txt \
        > workspace/server-demo.log 2>&1 &
    server_pid=$!

    for _ in $(seq 1 100); do
        if [[ -s workspace/port.txt ]]; then
            break
        fi
        sleep 0.02
    done

    port="$(cat workspace/port.txt)"
    ./build/netservice-client "$port" "network-service-ok"
    kill -TERM "$server_pid"
    wait "$server_pid"
    cat workspace/server-demo.log
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
