#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"
mkdir -p evidence
{
  echo "=== LAB 04-01 VALIDATION ==="
  date
  echo
  echo "=== CLEAN BUILD ==="
  make clean
  make
  echo
  echo "=== PROGRAM OUTPUT ==="
  output="$(./build/fork_demo)"
  printf '%s\n' "$output"
  echo
  echo "=== ASSERTIONS ==="
  grep -q 'value=42' <<<"$output"
  grep -q 'value=141' <<<"$output"
  echo "independent_post_fork_values=yes"
  echo
  echo "=== RESULT ==="
  echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
