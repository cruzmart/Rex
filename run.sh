#!/bin/bash
set -e

# Root of the project
ROOT=$(dirname "$0")

# Build directory
BUILD_DIR="$ROOT/build"

# Create build dir if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
    mkdir -p "$BUILD_DIR"
    cmake -G Ninja "$ROOT"
fi

# Incremental build (only rebuilds changed .cpp/.h files)
cmake --build "$BUILD_DIR"

# Run your executable
"$BUILD_DIR/src/rex" "$@"
