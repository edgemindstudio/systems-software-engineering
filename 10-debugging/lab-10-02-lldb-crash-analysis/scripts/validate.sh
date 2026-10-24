#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"
mkdir -p evidence

{
    echo "=== LAB 10-02 VALIDATION ==="
    date

    echo
    echo "=== CLEAN BUILD ==="
    make clean
    make

    echo
    echo "=== LLDB BATCH ANALYSIS ==="

    set +e
    timeout 15s lldb --batch \
        -o "settings set target.disable-aslr false" \
        -o run \
        -k bt \
        -k "process kill" \
        ./build/lldb_crash_demo > /tmp/lldb.out 2>&1
    rc=$?
    set -e

    cat /tmp/lldb.out

    grep -q 'stop reason = signal SIGSEGV' /tmp/lldb.out
    grep -q 'trigger_fault' /tmp/lldb.out
    grep -q 'main at main.c' /tmp/lldb.out

    if [[ "$rc" -ne 0 && "$rc" -ne 124 ]]; then
        echo "unexpected_lldb_return_code=$rc"
        rm -f /tmp/lldb.out
        exit 1
    fi

    echo "lldb_return_code=$rc"

    if [[ "$rc" -eq 124 ]]; then
        echo "lldb_wsl_batch_timeout_after_diagnostic=yes"
    else
        echo "lldb_clean_batch_exit=yes"
    fi

    echo "lldb_crash_backtrace_verified=yes"

    rm -f /tmp/lldb.out

    echo
    echo "=== RESULT ==="
    echo PASS
} > evidence/validation.txt 2>&1

cat evidence/validation.txt
