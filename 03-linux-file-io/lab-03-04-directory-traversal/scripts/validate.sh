#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"
rm -rf workspace
mkdir -p workspace/tree/subdir evidence
printf 'a\n' > workspace/tree/file-a.txt
printf 'b\n' > workspace/tree/file-b.txt
ln -s file-a.txt workspace/tree/link-a
{
  echo "=== LAB 03-04 VALIDATION ==="
  date
  echo
  echo "=== CLEAN BUILD ==="
  make clean
  make
  echo
  echo "=== DIRECTORY WALK ==="
  ./build/dir_walk workspace/tree | sort
  echo
  echo "=== ASSERTIONS ==="
  output="$(./build/dir_walk workspace/tree | sort)"
  grep -q $'^file-a.txt\tregular$' <<<"$output"
  grep -q $'^file-b.txt\tregular$' <<<"$output"
  grep -q $'^link-a\tsymlink$' <<<"$output"
  grep -q $'^subdir\tdirectory$' <<<"$output"
  echo "entry_classification_verified=yes"
  echo
  echo "=== RESULT ==="
  echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
