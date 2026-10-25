#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{ echo "=== INC-09 ==="; date; make clean; mkdir -p workspace; make; ./build/incident_09_sparse_file_space; du -h workspace/sparse.bin; du -h --apparent-size workspace/sparse.bin; } > evidence/validation.txt 2>&1
grep -q '^sparse_behavior_observed=yes$' evidence/validation.txt
echo PASS >> evidence/validation.txt
cat evidence/validation.txt
