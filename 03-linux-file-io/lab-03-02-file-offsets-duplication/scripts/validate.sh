#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"
rm -rf workspace
mkdir -p workspace evidence
printf 'ABCDE' > workspace/input.txt
{
  echo "=== LAB 03-02 VALIDATION ==="
  date
  echo
  echo "=== CLEAN BUILD ==="
  make clean
  make
  echo
  echo "=== PROGRAM OUTPUT ==="
  ./build/offset_demo workspace/input.txt
  echo
  echo "=== ASSERTIONS ==="
  output="$(./build/offset_demo workspace/input.txt)"
  grep -q '^fd1_byte=A$' <<<"$output"
  grep -q '^dup_fd2_byte=B$' <<<"$output"
  grep -q '^independent_fd3_byte=A$' <<<"$output"
  grep -q '^fd1_offset=2$' <<<"$output"
  grep -q '^fd2_offset=2$' <<<"$output"
  grep -q '^fd3_offset=1$' <<<"$output"
  echo "shared_offset_verified=yes"
  echo
  echo "=== RESULT ==="
  echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
