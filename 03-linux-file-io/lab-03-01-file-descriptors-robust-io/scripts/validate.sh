#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"
rm -rf workspace
mkdir -p workspace evidence
printf 'alpha\nbeta\ngamma\n' > workspace/source.txt
{
  echo "=== LAB 03-01 VALIDATION ==="
  date
  echo
  echo "=== CLEAN BUILD ==="
  make clean
  make
  echo
  echo "=== COPY ==="
  ./build/fd_copy workspace/source.txt workspace/destination.txt
  cmp workspace/source.txt workspace/destination.txt
  sha256sum workspace/source.txt workspace/destination.txt
  echo
  echo "=== STRACE SYSCALL SUMMARY ==="
  if command -v strace >/dev/null 2>&1; then
    strace -c -e trace=openat,read,write,close ./build/fd_copy workspace/source.txt workspace/trace-copy.txt
    cmp workspace/source.txt workspace/trace-copy.txt
  else
    echo "strace unavailable in this environment; syscall trace skipped"
  fi
  echo
  echo "=== RESULT ==="
  echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
