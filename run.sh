#!/bin/bash
set -e

ROOT=$(dirname "$0")
BUILD_DIR="$ROOT/build"

# Configure into build directory
cmake -S "$ROOT" -B "$BUILD_DIR" -G Ninja

# Build
cmake --build "$BUILD_DIR"

# Run executable
"$BUILD_DIR/src/rex" "$@"