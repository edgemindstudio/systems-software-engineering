#!/usr/bin/env bash
set -euo pipefail

BIN="${1:-./build/sysutil}"
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

rm -rf workspace
mkdir -p workspace/tree/sub

printf 'alpha\nbeta\n' > workspace/source.txt
printf 'nested-data\n' > workspace/tree/sub/nested.txt
printf '\x00\x41\x42\xff\x43\n' > workspace/binary.dat
ln -s sub/nested.txt workspace/tree/link-to-nested

echo "[test] help"
"$BIN" help | grep -q '^  sysutil stat PATH$'

echo "[test] stat regular file"
stat_out="$("$BIN" stat workspace/source.txt)"
grep -q '^type=regular$' <<<"$stat_out"
grep -q '^size=11$' <<<"$stat_out"

echo "[test] stat symlink without following"
link_out="$("$BIN" stat workspace/tree/link-to-nested)"
grep -q '^type=symlink$' <<<"$link_out"

echo "[test] copy"
copy_out="$("$BIN" copy workspace/source.txt workspace/copied.txt)"
grep -q '^copied_bytes=11$' <<<"$copy_out"
cmp workspace/source.txt workspace/copied.txt

echo "[test] walk"
walk_out="$("$BIN" walk workspace/tree)"
grep -q 'type=directory path=workspace/tree$' <<<"$walk_out"
grep -q 'type=directory path=workspace/tree/sub$' <<<"$walk_out"
grep -q 'type=regular path=workspace/tree/sub/nested.txt$' <<<"$walk_out"
grep -q 'type=symlink path=workspace/tree/link-to-nested$' <<<"$walk_out"

echo "[test] hex bounded output"
hex_out="$("$BIN" hex workspace/binary.dat 5)"
grep -q '^00000000  00 41 42 ff 43' <<<"$hex_out"
grep -q '^displayed_bytes=5$' <<<"$hex_out"

echo "[test] proc self"
proc_out="$("$BIN" proc self)"
grep -q '^pid=self$' <<<"$proc_out"
grep -q '^Name:' <<<"$proc_out"
grep -q '^State:' <<<"$proc_out"
grep -q '^Threads:' <<<"$proc_out"
grep -Eq '^reported_fields=[34]$' <<<"$proc_out"

echo "[test] invalid command"
set +e
"$BIN" definitely-not-a-command >/tmp/sysutil-invalid.out 2>&1
invalid_rc=$?
set -e
test "$invalid_rc" -eq 2
grep -q "unknown command" /tmp/sysutil-invalid.out
rm -f /tmp/sysutil-invalid.out

echo "integration_tests=PASS"
