#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{ echo "=== INC-06 ==="; date; make clean; make; ./build/incident_06_tcp_framing_failure; } > evidence/validation.txt 2>&1
grep -q '^stream_payload=HELLOWORLD$' evidence/validation.txt
grep -q '^framing_required=yes$' evidence/validation.txt
echo PASS >> evidence/validation.txt
cat evidence/validation.txt
