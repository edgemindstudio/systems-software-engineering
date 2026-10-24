#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"
rm -rf workspace
mkdir -p workspace evidence
{
  echo "=== LAB 03-05 VALIDATION ==="
  date
  echo
  echo "=== CLEAN BUILD ==="
  make clean
  make
  echo
  echo "=== PROGRAM OUTPUT ==="
  ./build/sparse_demo workspace/sparse.bin
  echo
  echo "=== FILESYSTEM VIEW ==="
  ls -ls workspace/sparse.bin
  du -B1 workspace/sparse.bin
  stat -c 'size=%s blocks=%b block_size=%B' workspace/sparse.bin
  echo
  echo "=== ASSERTIONS ==="
  logical="$(stat -c %s workspace/sparse.bin)"
  allocated="$(( $(stat -c %b workspace/sparse.bin) * 512 ))"
  echo "logical_bytes=$logical"
  echo "allocated_bytes=$allocated"
  test "$logical" -gt "$allocated"
  echo "sparse_allocation_verified=yes"
  echo
  echo "=== RESULT ==="
  echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
