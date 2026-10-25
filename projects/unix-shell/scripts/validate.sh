#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"; mkdir -p evidence
{
echo "========================================"
echo "UNIX SHELL VALIDATION"
echo "========================================"
date; echo
echo "=== TOOLCHAIN ==="
clang --version | head -n 1
uname -srmo; echo
echo "=== STRICT BUILD ==="
make clean; make; echo
echo "=== INTEGRATION TESTS ==="
make test; echo
echo "=== REPRESENTATIVE SIMPLE COMMAND ==="
./build/sshell -c "printf 'shell-ok\n'"; echo
echo "=== REPRESENTATIVE PIPELINE ==="
./build/sshell -c "printf 'alpha\nbeta\n' | grep beta | tr a-z A-Z"; echo
echo "=== REPRESENTATIVE REDIRECTION ==="
mkdir -p workspace
./build/sshell -c "printf 'redirected\n' > workspace/demo.txt"
./build/sshell -c "cat < workspace/demo.txt"; echo
echo "=== REPRESENTATIVE BUILTINS ==="
./build/sshell -c pwd
./build/sshell -c help; echo
echo "=== SANITIZER VALIDATION ==="
make sanitize; echo
echo "=== RESTORE NORMAL BUILD ==="
make clean; make; echo
echo "=== RESULT ==="
echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
