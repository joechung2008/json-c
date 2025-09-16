#!/usr/bin/env bash
# Configure and build the project (POSIX)
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="$ROOT_DIR/build"
mkdir -p "$BUILD_DIR"
cmake -S "$ROOT_DIR" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Debug -Wno-dev
cmake --build "$BUILD_DIR" --parallel

echo "Build complete. Build artifacts are in: $BUILD_DIR"