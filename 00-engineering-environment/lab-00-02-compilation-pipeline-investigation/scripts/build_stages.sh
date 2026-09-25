#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SRC_DIR="$ROOT_DIR/src"
ARTIFACT_DIR="$ROOT_DIR/artifacts"

mkdir -p "$ARTIFACT_DIR"

COMMON_FLAGS=(
    -std=c17
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wshadow
    -Wformat=2
    -Wundef
    -g3
    -O0
)

PREPROCESS_FLAGS=(
    "${COMMON_FLAGS[@]}"
    -I"$SRC_DIR"
)

echo "[1/5] Preprocessing"

clang "${PREPROCESS_FLAGS[@]}" \
    -E "$SRC_DIR/main.c" \
    -o "$ARTIFACT_DIR/main.i"

clang "${PREPROCESS_FLAGS[@]}" \
    -E "$SRC_DIR/math_ops.c" \
    -o "$ARTIFACT_DIR/math_ops.i"

echo "[2/5] Compiling to assembly"

clang "${COMMON_FLAGS[@]}" \
    -S "$ARTIFACT_DIR/main.i" \
    -o "$ARTIFACT_DIR/main.s"

clang "${COMMON_FLAGS[@]}" \
    -S "$ARTIFACT_DIR/math_ops.i" \
    -o "$ARTIFACT_DIR/math_ops.s"

echo "[3/5] Assembling"

clang -c "$ARTIFACT_DIR/main.s" \
    -o "$ARTIFACT_DIR/main.o"

clang -c "$ARTIFACT_DIR/math_ops.s" \
    -o "$ARTIFACT_DIR/math_ops.o"

echo "[4/5] Linking"

clang \
    "$ARTIFACT_DIR/main.o" \
    "$ARTIFACT_DIR/math_ops.o" \
    -o "$ARTIFACT_DIR/pipeline_demo"

echo "[5/5] Running"

"$ARTIFACT_DIR/pipeline_demo"

echo
echo "Build pipeline complete."
