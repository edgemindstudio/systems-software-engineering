#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{
 echo "=== INC-07 ==="; date; make clean; make
 echo "=== FAILURE WITHOUT SEARCH PATH ==="
 set +e
 build/bin/demo
 fail_rc=$?
 set -e
 echo "failure_exit_code=$fail_rc"
 echo "=== LDD BEFORE REMEDIATION ==="
 ldd build/bin/demo || true
 echo "=== REMEDIATION WITH LD_LIBRARY_PATH ==="
 output="$(LD_LIBRARY_PATH="$ROOT/build/lib" build/bin/demo)"
 printf '%s\n' "$output"
 echo "=== LDD WITH SEARCH PATH ==="
 LD_LIBRARY_PATH="$ROOT/build/lib" ldd build/bin/demo
 grep -q '^demo_value=77$' <<<"$output"
 test "$fail_rc" -ne 0
 echo "lookup_failure_reproduced=yes"
 echo "runtime_search_path_remediation_verified=yes"
 echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
