#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

cd "$ROOT_DIR"

echo "========================================"
echo "BUILD SYSTEM FOUNDATIONS"
echo "========================================"

echo
echo "[1/4] GNU Make clean build"
make clean
make
make test

echo
echo "[2/4] Configure CMake Debug with Ninja"
cmake \
    -S . \
    -B build/cmake-debug \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=Debug

echo
echo "[3/4] Build CMake Debug"
cmake --build build/cmake-debug

echo
echo "[4/4] Run CMake Debug executable"
./build/cmake-debug/build_demo

echo
echo "========================================"
echo "ALL BUILDS PASSED"
echo "========================================"
