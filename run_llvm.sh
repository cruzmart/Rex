#!/bin/bash
set -e

ROOT=$(cd "$(dirname "$0")" && pwd)

BUILD_DIR="$ROOT/build"
LLVM_DIR="$BUILD_DIR/llvm"
RUNTIME_DIR="$ROOT/runtime"

mkdir -p "$LLVM_DIR"

MLIR_FILE="$ROOT/output.ll"
OBJ_FILE="$LLVM_DIR/output.o"
RUNTIME_OBJ="$LLVM_DIR/runtime.o"
EXE_FILE="$LLVM_DIR/output"

# -----------------------------
# 1. Compile LLVM IR -> object
# -----------------------------
llc -filetype=obj -relocation-model=pic "$MLIR_FILE" -o "$OBJ_FILE"

# -----------------------------
# 2. Compile runtime -> object
# -----------------------------
g++ -c "$RUNTIME_DIR/rex_runtime.cpp" -o "$RUNTIME_OBJ"

# -----------------------------
# 3. Link everything together
# -----------------------------
clang++ "$OBJ_FILE" "$RUNTIME_OBJ" -o "$EXE_FILE"

# -----------------------------
# 4. Run
# -----------------------------
"$EXE_FILE"