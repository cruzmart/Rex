#!/bin/bash
set -e

ROOT=$(cd "$(dirname "$0")" && pwd)
BUILD_DIR="$ROOT/build"
LLVM_DIR="$BUILD_DIR/llvm"

mkdir -p "$LLVM_DIR"

MLIR_FILE="$ROOT/output.ll"
OBJ_FILE="$LLVM_DIR/output.o"
EXE_FILE="$LLVM_DIR/output"

# Compile LLVM IR -> object file
llc -filetype=obj -relocation-model=pic "$MLIR_FILE" -o "$OBJ_FILE"

# Link object -> executable
clang "$OBJ_FILE" -o "$EXE_FILE"

# Run
"$EXE_FILE"