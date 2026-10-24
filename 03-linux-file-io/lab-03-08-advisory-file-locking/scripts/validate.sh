#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"
rm -rf workspace
mkdir -p workspace evidence
: > workspace/lockfile
{
  echo "=== LAB 03-08 VALIDATION ==="
  date
  echo
  echo "=== CLEAN BUILD ==="
  make clean
  make
  echo
  echo "=== START LOCK HOLDER ==="
  ./build/lock_demo workspace/lockfile hold 2 > workspace/holder.txt 2>&1 &
  holder_pid=$!
  sleep 0.3
  cat workspace/holder.txt
  echo
  echo "=== CONTENDING TRY ==="
  set +e
  contender_output="$(./build/lock_demo workspace/lockfile try 2>&1)"
  contender_status=$?
  set -e
  printf '%s\n' "$contender_output"
  echo "contender_exit_status=$contender_status"
  test "$contender_status" -eq 3
  grep -q '^lock=busy$' <<<"$contender_output"
  echo
  echo "=== WAIT FOR RELEASE ==="
  wait "$holder_pid"
  echo
  echo "=== TRY AFTER RELEASE ==="
  released_output="$(./build/lock_demo workspace/lockfile try)"
  printf '%s\n' "$released_output"
  grep -q '^lock=acquired ' <<<"$released_output"
  echo
  echo "=== RESULT ==="
  echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
