#!/bin/bash
set -e

ROOT=$(cd "$(dirname "$0")" && pwd)
BUILD_DIR="$ROOT/build"

# Configure (only if needed)
if [ ! -f "$BUILD_DIR/build.ninja" ]; then
  cmake -S "$ROOT" -B "$BUILD_DIR" -G Ninja
fi

# Build
cmake --build "$BUILD_DIR"

# Run executable
"$BUILD_DIR/src/rex" "$@"