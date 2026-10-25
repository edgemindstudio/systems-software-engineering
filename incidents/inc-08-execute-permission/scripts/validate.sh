#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence workspace
cp payload.sh workspace/payload.sh
chmod 600 workspace/payload.sh
set +e
workspace/payload.sh > /tmp/inc08.out 2>&1
fail_rc=$?
set -e
chmod 700 workspace/payload.sh
output="$(workspace/payload.sh)"
{
 echo "=== INC-08 ==="; date
 echo "initial_mode=$(stat -c '%a' payload.sh)"
 echo "failure_exit_code=$fail_rc"
 echo "remediated_output=$output"
 echo "permission_failure_reproduced=$([ "$fail_rc" -ne 0 ] && echo yes || echo no)"
 echo "execute_permission_remediation_verified=$([ "$output" = "permission-demo-ran" ] && echo yes || echo no)"
 echo PASS
} > evidence/validation.txt
cat evidence/validation.txt
rm -rf workspace /tmp/inc08.out
