#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"
rm -rf workspace
mkdir -p workspace evidence
printf 'metadata\n' > workspace/original.txt
ln workspace/original.txt workspace/hard.txt
ln -s original.txt workspace/symbolic.txt
{
  echo "=== LAB 03-03 VALIDATION ==="
  date
  echo
  echo "=== CLEAN BUILD ==="
  make clean
  make
  echo
  echo "=== ORIGINAL ==="
  ./build/metadata_demo workspace/original.txt
  echo
  echo "=== HARD LINK ==="
  ./build/metadata_demo workspace/hard.txt
  echo
  echo "=== SYMBOLIC LINK ==="
  ./build/metadata_demo workspace/symbolic.txt
  echo
  echo "=== STAT COMMAND ==="
  stat -c '%n inode=%i links=%h size=%s type=%F' workspace/original.txt workspace/hard.txt workspace/symbolic.txt
  echo
  echo "=== ASSERTIONS ==="
  test "$(stat -c %i workspace/original.txt)" = "$(stat -c %i workspace/hard.txt)"
  test "$(stat -c %h workspace/original.txt)" -eq 2
  test -L workspace/symbolic.txt
  echo "hardlink_inode_shared=yes"
  echo "symlink_detected=yes"
  echo
  echo "=== RESULT ==="
  echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
