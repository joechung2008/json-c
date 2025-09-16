#!/usr/bin/env bash
# Configure and build the project (POSIX)
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="$ROOT_DIR/build"
mkdir -p "$BUILD_DIR"

# Allow overriding whether to build tests via environment variable.
# Default to ON for developer builds but allow CI or callers to set it to OFF.
JSON_C_BUILD_TESTS=${JSON_C_BUILD_TESTS:-ON}
echo "Configuring build (tests: $JSON_C_BUILD_TESTS)"
cmake -S "$ROOT_DIR" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Debug -DJSON_C_BUILD_TESTS=$JSON_C_BUILD_TESTS -Wno-dev
cmake --build "$BUILD_DIR" --parallel

echo "Build complete. Build artifacts are in: $BUILD_DIR"