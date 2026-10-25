#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
set +e
{ echo "=== INC-04 ==="; date; make clean; make; ASAN_OPTIONS=detect_leaks=0 ./build/incident_04_use_after_free; } > evidence/validation.txt 2>&1
rc=$?
set -e
grep -q 'AddressSanitizer' evidence/validation.txt
grep -q 'heap-use-after-free' evidence/validation.txt
printf 'asan_exit_code=%d\nPASS\n' "$rc" >> evidence/validation.txt
cat evidence/validation.txt
