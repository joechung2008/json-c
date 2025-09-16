#!/usr/bin/env bash
# Build (if needed) and run ctest (POSIX)
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="$ROOT_DIR/build"
if [ ! -d "$BUILD_DIR" ]; then
  echo "Build directory not found; running build first..."
  "$ROOT_DIR/bin/build.sh"
fi
ctest --test-dir "$BUILD_DIR" --output-on-failure