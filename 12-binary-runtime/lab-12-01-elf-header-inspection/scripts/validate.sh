#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"; cd "$ROOT"; mkdir -p evidence
{
 echo "=== LAB 12-01 VALIDATION ==="; date
 echo; echo "=== CLEAN BUILD ==="; make clean; make
 echo; echo "=== ELF HEADER ==="
 readelf -h ./build/elf_header_demo
 echo; echo "=== FILE TYPE ==="
 file ./build/elf_header_demo
 readelf -h ./build/elf_header_demo | grep -q 'ELF64'
 readelf -h ./build/elf_header_demo | grep -q 'Advanced Micro Devices X86-64'
 echo "elf_header_verified=yes"
 echo; echo "=== RESULT ==="; echo PASS
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
