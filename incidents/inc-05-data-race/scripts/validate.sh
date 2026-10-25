#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
set +e
{ echo "=== INC-05 ==="; date; make clean; make; TSAN_OPTIONS=halt_on_error=1 ./build/incident_05_data_race; } > evidence/validation.txt 2>&1
rc=$?
set -e
grep -q 'ThreadSanitizer' evidence/validation.txt
grep -q -i 'data race' evidence/validation.txt
printf 'tsan_exit_code=%d\nPASS\n' "$rc" >> evidence/validation.txt
cat evidence/validation.txt
