#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"
mkdir -p evidence
{
  echo "=== LAB 04-03 VALIDATION ==="
  date
  echo
  echo "=== CLEAN BUILD ==="
  make clean
  make
  echo
  echo "=== PROGRAM OUTPUT ==="
  output="$(./build/reap_demo)"
  printf '%s\n' "$output"
  echo
  echo "=== ASSERTIONS ==="
  grep -q 'exit=11' <<<"$output"
  grep -q 'exit=22' <<<"$output"
  grep -q 'exit=33' <<<"$output"
  test "$(grep -c '^reaped pid=' <<<"$output")" -eq 3
  echo "three_children_reaped=yes"
  echo
  echo "=== RESULT ==="
  echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
