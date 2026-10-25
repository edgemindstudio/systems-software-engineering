#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 12-03 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== SYMBOL TABLE ==="
 nm ./build/relocations.o
 echo; echo "=== RELOCATIONS ==="
 readelf -r ./build/relocations.o
 nm ./build/relocations.o | grep -Eq '[[:space:]]U[[:space:]]+external_transform$'
 readelf -r ./build/relocations.o | grep -q 'external_transform'
 echo "relocation_external_symbol_verified=yes"
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
