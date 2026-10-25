#!/usr/bin/env bash
set -euo pipefail
BIN="${1:-./build/sshell}"
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"
rm -rf workspace
mkdir -p workspace/subdir

echo "[test] external"
test "$("$BIN" -c 'printf hello')" = "hello"
echo "[test] single quotes"
test "$("$BIN" -c "printf '%s' 'hello world'")" = "hello world"
echo "[test] double quotes"
test "$("$BIN" -c 'printf "%s" "two words"')" = "two words"
echo "[test] escape"
test "$("$BIN" -c 'printf "%s" one\ two')" = "one two"
echo "[test] pipeline"
test "$("$BIN" -c "printf 'alpha\nbeta\n' | grep beta | tr a-z A-Z")" = "BETA"
echo "[test] output redirect"
"$BIN" -c "printf 'first\n' > workspace/output.txt"
grep -qx first workspace/output.txt
echo "[test] append redirect"
"$BIN" -c "printf 'second\n' >> workspace/output.txt"
test "$(cat workspace/output.txt)" = $'first\nsecond'
echo "[test] input redirect"
test "$("$BIN" -c 'cat < workspace/output.txt | tail -n 1')" = "second"
echo "[test] pwd builtin"
test "$("$BIN" -c pwd)" = "$ROOT"
echo "[test] cd persistence"
test "$(printf 'cd workspace/subdir\npwd\nexit 0\n' | "$BIN")" = "$ROOT/workspace/subdir"
echo "[test] builtin redirect"
"$BIN" -c 'pwd > workspace/pwd.txt'
grep -qx "$ROOT" workspace/pwd.txt
echo "[test] status"
test "$(printf 'false\nstatus\nexit 0\n' | "$BIN")" = "1"
echo "[test] not found"
set +e; "$BIN" -c /definitely_not_a_real_command_xyz >/tmp/sshell-nf 2>&1; rc=$?; set -e
test "$rc" -eq 127; grep -q execvp /tmp/sshell-nf
echo "[test] explicit exit"
set +e; "$BIN" -c 'exit 37' >/tmp/sshell-ex 2>&1; rc=$?; set -e
test "$rc" -eq 37
echo "[test] malformed pipeline"
set +e; "$BIN" -c 'echo hello |' >/tmp/sshell-pe 2>&1; rc=$?; set -e
test "$rc" -eq 2; grep -q "pipeline ends with empty command" /tmp/sshell-pe
rm -f /tmp/sshell-nf /tmp/sshell-ex /tmp/sshell-pe
echo "integration_tests=PASS"
