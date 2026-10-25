#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 12-02 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== PROGRAM OUTPUT ==="; ./build/sections_symbols_demo
 echo; echo "=== SELECTED SECTIONS ==="
 readelf -S ./build/sections_symbols_demo | grep -E '\.(text|data|bss)[[:space:]]'
 echo; echo "=== SELECTED SYMBOLS ==="
 nm -C ./build/sections_symbols_demo | grep -E 'exported_add|initialized_global|zero_initialized_global'
 readelf -S ./build/sections_symbols_demo | grep -q '\.text'
 readelf -S ./build/sections_symbols_demo | grep -q '\.data'
 readelf -S ./build/sections_symbols_demo | grep -q '\.bss'
 nm ./build/sections_symbols_demo | grep -q 'exported_add'
 echo "sections_symbols_verified=yes"
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
