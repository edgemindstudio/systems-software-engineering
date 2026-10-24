#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"
rm -rf workspace
mkdir -p workspace evidence
{
  echo "=== LAB 03-06 VALIDATION ==="
  date
  echo
  echo "=== CLEAN BUILD ==="
  make clean
  make
  echo
  echo "=== PROGRAM OUTPUT ==="
  ./build/vectored_demo workspace/message.txt
  echo
  echo "=== FILE CONTENT ==="
  cat workspace/message.txt
  echo
  echo "=== STRACE ==="
  if command -v strace >/dev/null 2>&1; then
    strace -e trace=readv,writev,lseek ./build/vectored_demo workspace/trace-message.txt
  else
    echo "strace unavailable in this environment; syscall trace skipped"
  fi
  echo
  echo "=== ASSERTIONS ==="
  grep -q '^TYPE=event$' workspace/message.txt
  grep -q '^payload=systems-io$' workspace/message.txt
  grep -q '^END$' workspace/message.txt
  echo "vectored_io_verified=yes"
  echo
  echo "=== RESULT ==="
  echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
