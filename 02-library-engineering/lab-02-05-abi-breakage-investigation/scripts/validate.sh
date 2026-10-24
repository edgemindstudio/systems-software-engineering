#!/usr/bin/env bash
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$ROOT_DIR"
mkdir -p evidence
{
    echo "=== LAB 02-05 VALIDATION ==="
    date
    echo
    echo "=== CLEAN BUILD ==="
    make clean
    make
    echo
    echo "=== V1 CLIENT RUN ==="
    LD_LIBRARY_PATH=build/v1 ./build/bin/record_consumer
    echo
    echo "=== V2 CLIENT RUN (EXPECTED ABI REJECTION) ==="
    set +e
    LD_LIBRARY_PATH=build/v2 ./build/bin/record_consumer > build/v2-validation-output.txt 2>&1
    status=$?
    set -e
    cat build/v2-validation-output.txt
    echo "v2_exit_status=$status"
    test "$status" -eq 2
    grep -q "ABI MISMATCH" build/v2-validation-output.txt
    echo
    echo "=== V1 PUBLIC STRUCT SIZE ==="
    printf 'consumer_compiled_size='
    LD_LIBRARY_PATH=build/v1 ./build/bin/record_consumer | sed -n 's/^consumer_struct_size=//p'
    echo
    echo "=== SONAME COMPARISON ==="
    readelf -d build/v1/librecord.so.1.0.0 | grep SONAME
    readelf -d build/v2/librecord.so.1.0.0 | grep SONAME
    echo
    echo "=== SYMBOL COMPARISON ==="
    echo "-- v1 --"
    nm -D --defined-only build/v1/librecord.so.1.0.0
    echo "-- v2 --"
    nm -D --defined-only build/v2/librecord.so.1.0.0
    echo
    echo "=== MAKE TEST ==="
    make test
    echo
    echo "=== RESULT ==="
    echo "PASS"
} > evidence/validation.txt 2>&1
cat evidence/validation.txt
