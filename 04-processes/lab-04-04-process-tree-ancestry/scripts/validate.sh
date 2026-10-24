#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"
mkdir -p evidence
{
  echo "=== LAB 04-04 VALIDATION ==="
  date
  echo
  echo "=== CLEAN BUILD ==="
  make clean
  make
  echo
  echo "=== PROGRAM OUTPUT ==="
  output="$(./build/tree_demo)"
  printf '%s\n' "$output"
  echo
  echo "=== ASSERTIONS ==="
  root_pid="$(sed -n 's/^root pid=\([0-9][0-9]*\).*/\1/p' <<<"$output")"
  child_pid="$(sed -n 's/^child pid=\([0-9][0-9]*\).*/\1/p' <<<"$output")"
  child_ppid="$(sed -n 's/^child pid=[0-9][0-9]* ppid=\([0-9][0-9]*\).*/\1/p' <<<"$output")"
  grand_ppid="$(sed -n 's/^grandchild pid=[0-9][0-9]* ppid=\([0-9][0-9]*\).*/\1/p' <<<"$output")"
  test "$child_ppid" = "$root_pid"
  test "$grand_ppid" = "$child_pid"
  echo "three_generation_ancestry_verified=yes"
  echo
  echo "=== RESULT ==="
  echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
