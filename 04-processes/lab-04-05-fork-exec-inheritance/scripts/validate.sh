#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"
rm -rf workspace
mkdir -p workspace evidence
{
  echo "=== LAB 04-05 VALIDATION ==="
  date
  echo
  echo "=== CLEAN BUILD ==="
  make clean
  make
  echo
  echo "=== PROGRAM OUTPUT ==="
  output="$(./build/inherit_launcher)"
  printf '%s\n' "$output"
  echo
  echo "=== ASSERTIONS ==="
  grep -q '^env_token=visible-after-exec$' <<<"$output"
  grep -q '^inherited_fd_open=yes$' <<<"$output"
  grep -q '^cloexec_fd_open=no$' <<<"$output"
  grep -q "^cwd=$ROOT$" <<<"$output"
  echo "cwd_environment_fd_inheritance_verified=yes"
  echo "cloexec_verified=yes"
  echo
  echo "=== RESULT ==="
  echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
