#!/bin/bash
set -e

MLIR_FILE="output.ll"
OBJ_FILE="output.o"
EXE_FILE="output"

# Lower MLIR -> LLVM IR (optional, if dumpLLVM already gives LLVM IR)
# mlir-translate --mlir-to-llvmir "$MLIR_FILE" > "${MLIR_FILE%.ll}.llvm.ll"

# Compile LLVM IR -> object file (PIC)
llc -filetype=obj -relocation-model=pic "$MLIR_FILE" -o "$OBJ_FILE"

# Link object -> executable
clang "$OBJ_FILE" -o "$EXE_FILE"  # no need for -no-pie if you used PIC

# Run executable
./"$EXE_FILE"