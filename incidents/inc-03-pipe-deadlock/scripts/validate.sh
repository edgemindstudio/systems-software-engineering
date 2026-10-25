#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{ echo "=== INC-03 ==="; date; make clean; make; ./build/incident_03_pipe_deadlock; } > evidence/validation.txt 2>&1
grep -q '^writer_still_open_no_eof=yes$' evidence/validation.txt
grep -q '^eof_after_writer_close=yes$' evidence/validation.txt
echo PASS >> evidence/validation.txt
cat evidence/validation.txt
