#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"
mkdir -p evidence
{
  echo "=== LAB 04-02 VALIDATION ==="
  date
  echo
  echo "=== CLEAN BUILD ==="
  make clean
  make
  echo
  echo "=== PROGRAM OUTPUT ==="
  output="$(./build/exec_launcher)"
  printf '%s\n' "$output"
  echo
  echo "=== ASSERTIONS ==="
  pre="$(sed -n 's/^pre_exec_pid=//p' <<<"$output")"
  post="$(sed -n 's/^post_exec_pid=//p' <<<"$output")"
  test -n "$pre"
  test "$pre" = "$post"
  grep -q '^argc=3$' <<<"$output"
  grep -q '^argv1=alpha$' <<<"$output"
  grep -q '^argv2=beta$' <<<"$output"
  grep -q '^env_token=exec-preserved-env$' <<<"$output"
  echo "pid_preserved_across_exec=yes"
  echo
  echo "=== STRACE EXEC ==="
  if command -v strace >/dev/null 2>&1; then
    strace -f -e trace=process ./build/exec_launcher 2>&1 | grep -E 'clone|fork|vfork|execve|wait4|waitid' || true
  else
    echo "strace unavailable; process trace skipped"
  fi
  echo
  echo "=== RESULT ==="
  echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
