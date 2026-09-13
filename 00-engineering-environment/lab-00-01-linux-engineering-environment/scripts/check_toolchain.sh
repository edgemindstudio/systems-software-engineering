#!/usr/bin/env bash

set -u
set -o pipefail

check_first_line() {
    local tool="$1"
    shift

    if command -v "$tool" >/dev/null 2>&1; then
        "$@" 2>&1 | head -n 1
    else
        echo "$tool: not installed"
    fi
}

echo "========================================"
echo "SYSTEMS ENGINEERING ENVIRONMENT CHECK"
echo "========================================"
echo

echo "[SYSTEM]"
uname -a
echo

echo "[OS]"
if [ -f /etc/os-release ]; then
    cat /etc/os-release
else
    echo "/etc/os-release: not available"
fi
echo

echo "[ARCHITECTURE]"
uname -m
echo

echo "[SHELL]"
echo "${SHELL:-unknown}"
echo

echo "[COMPILERS]"
check_first_line gcc gcc --version
check_first_line clang clang --version
check_first_line g++ g++ --version
check_first_line clang++ clang++ --version
echo

echo "[BUILD TOOLS]"
check_first_line cmake cmake --version

if command -v ninja >/dev/null 2>&1; then
    echo "ninja $(ninja --version)"
else
    echo "ninja: not installed"
fi

check_first_line make make --version
echo

echo "[DEBUGGERS]"
check_first_line gdb gdb --version
check_first_line lldb lldb --version
echo

echo "[STATIC ANALYSIS]"
check_first_line clang-tidy clang-tidy --version

if command -v cppcheck >/dev/null 2>&1; then
    cppcheck --version
else
    echo "cppcheck: not installed"
fi
echo

echo "[MEMORY / CORRECTNESS TOOLS]"
if command -v valgrind >/dev/null 2>&1; then
    valgrind --version
else
    echo "valgrind: not installed"
fi
echo

echo "[TRACING / PERFORMANCE]"
check_first_line strace strace --version
check_first_line ltrace ltrace --version

if command -v perf >/dev/null 2>&1; then
    perf --version
else
    echo "perf: not installed"
fi
echo

echo "[BINARY TOOLS]"
check_first_line objdump objdump --version
check_first_line readelf readelf --version
check_first_line nm nm --version

if command -v xxd >/dev/null 2>&1; then
    echo "xxd: installed ($(command -v xxd))"
else
    echo "xxd: not installed"
fi
echo

echo "[VERSION CONTROL]"
if command -v git >/dev/null 2>&1; then
    git --version
else
    echo "git: not installed"
fi

check_first_line gh gh --version
echo

echo "========================================"
echo "CHECK COMPLETE"
echo "========================================"