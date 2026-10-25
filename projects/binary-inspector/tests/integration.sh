#!/usr/bin/env bash
set -euo pipefail

BIN="${1:-./build/binspect}"
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT"

mkdir -p workspace
FIXTURE=workspace/fixture

clang -std=c17 -g -O0 tests/fixture.c -o "$FIXTURE"

echo "[test] summary"
summary="$("$BIN" "$FIXTURE" summary)"
grep -q '^class=ELF64$' <<<"$summary"
grep -q '^endianness=little$' <<<"$summary"
grep -Eq '^type=(DYN|EXEC)$' <<<"$summary"

echo "[test] sections"
sections="$("$BIN" "$FIXTURE" sections)"
grep -q $'\t.text\t' <<<"$sections"
grep -q $'\t.symtab\t' <<<"$sections"

echo "[test] segments"
segments="$("$BIN" "$FIXTURE" segments)"
grep -q $'\tLOAD\t' <<<"$segments"

echo "[test] symbols"
symbols="$("$BIN" "$FIXTURE" symbols)"
grep -q '^add_values' <<<"$symbols"
grep -q '^global_counter' <<<"$symbols"

echo "[test] needed"
needed="$("$BIN" "$FIXTURE" needed)"
readelf -d "$FIXTURE" | grep 'NEEDED' >/dev/null
while IFS= read -r lib; do
    grep -Fq "$lib" <<<"$needed"
done < <(readelf -d "$FIXTURE" | sed -n 's/.*Shared library: \[\(.*\)\]/\1/p')

echo "[test] readelf header parity"
our_sections="$(awk -F= '/^section_headers=/{print $2}' <<<"$summary")"
ref_sections="$(readelf -h "$FIXTURE" | awk '/Number of section headers:/{print $5}')"
test "$our_sections" = "$ref_sections"

our_ph="$(awk -F= '/^program_headers=/{print $2}' <<<"$summary")"
ref_ph="$(readelf -h "$FIXTURE" | awk '/Number of program headers:/{print $5}')"
test "$our_ph" = "$ref_ph"

echo "[test] mapping roundtrip"
text_line="$(awk -F'\t' '$2==".text"{print; exit}' <<<"$sections")"
text_offset="$(sed -n 's/.*offset=\(0x[0-9a-fA-F]*\).*/\1/p' <<<"$text_line")"
mapped_vaddr="$("$BIN" "$FIXTURE" offset-to-vaddr "$text_offset")"
mapped_offset="$("$BIN" "$FIXTURE" vaddr-to-offset "$mapped_vaddr")"
test "$mapped_offset" = "$text_offset"

echo "[test] malformed/truncated file"
head -c 32 "$FIXTURE" > workspace/truncated
set +e
"$BIN" workspace/truncated summary >/tmp/binspect-bad.out 2>&1
rc=$?
set -e
test "$rc" -ne 0
grep -q 'invalid or unsupported ELF file' /tmp/binspect-bad.out
rm -f /tmp/binspect-bad.out

echo "[test] non-ELF rejection"
printf 'not-elf\n' > workspace/not-elf
set +e
"$BIN" workspace/not-elf summary >/tmp/binspect-notelf.out 2>&1
rc=$?
set -e
test "$rc" -ne 0
rm -f /tmp/binspect-notelf.out

echo "binary_inspector_tests=PASS"
