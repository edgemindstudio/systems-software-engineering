#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"
rm -rf workspace
mkdir -p workspace evidence
printf 'hello from mmap validation\n' > workspace/mapped.txt
{
  echo "=== LAB 03-07 VALIDATION ==="
  date
  echo
  echo "=== CLEAN BUILD ==="
  make clean
  make
  echo
  echo "=== BEFORE ==="
  cat workspace/mapped.txt
  echo
  echo "=== PROGRAM OUTPUT ==="
  ./build/mmap_demo workspace/mapped.txt
  echo
  echo "=== AFTER ==="
  cat workspace/mapped.txt
  echo
  echo "=== ASSERTIONS ==="
  grep -q '^MMAP!' workspace/mapped.txt
  echo "mapped_mutation_persisted=yes"
  echo
  echo "=== STRACE MMAP/MSYNC ==="
  if command -v strace >/dev/null 2>&1; then
    strace -e trace=mmap,msync,munmap ./build/mmap_demo workspace/mapped.txt
  else
    echo "strace unavailable in this environment; syscall trace skipped"
  fi
  echo
  echo "=== RESULT ==="
  echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
