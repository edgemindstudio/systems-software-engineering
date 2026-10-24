#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"
mkdir -p evidence
{
  echo "=== LAB 04-06 VALIDATION ==="
  date
  echo
  echo "=== CLEAN BUILD ==="
  make clean
  make
  echo
  echo "=== PROGRAM OUTPUT ==="
  output="$(./build/group_session_demo)"
  printf '%s\n' "$output"
  echo
  echo "=== ASSERTIONS ==="
  group_pid="$(sed -n 's/^group_child pid=\([0-9][0-9]*\).*/\1/p' <<<"$output")"
  group_pgid="$(sed -n 's/^group_child .* pgid=\([0-9][0-9]*\).*/\1/p' <<<"$output")"
  session_pid="$(sed -n 's/^session_child pid=\([0-9][0-9]*\).*/\1/p' <<<"$output")"
  session_pgid="$(sed -n 's/^session_child .* pgid=\([0-9][0-9]*\).*/\1/p' <<<"$output")"
  session_sid="$(sed -n 's/^session_child .* sid=\([0-9][0-9]*\).*/\1/p' <<<"$output")"
  test "$group_pid" = "$group_pgid"
  test "$session_pid" = "$session_pgid"
  test "$session_pid" = "$session_sid"
  echo "new_process_group_verified=yes"
  echo "new_session_verified=yes"
  echo
  echo "=== RESULT ==="
  echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
