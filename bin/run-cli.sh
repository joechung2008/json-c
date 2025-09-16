#!/usr/bin/env bash
# Run the CLI (POSIX). Ensures dynamic loader can find the shared library on Linux/macOS
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="$ROOT_DIR/build"
EXE="$BUILD_DIR/json-cli"
if [ ! -x "$EXE" ]; then
  echo "Executable not found: $EXE"
  echo "Run bin/build.sh first."
  exit 1
fi
# Use LD_LIBRARY_PATH on Linux, DYLD_LIBRARY_PATH on macOS. PowerShell/Windows has separate scripts.
OS_NAME="$(uname -s)"
case "$OS_NAME" in
  Linux)
    LD_LIBRARY_PATH="$BUILD_DIR" "$EXE"
    ;;
  Darwin)
    DYLD_LIBRARY_PATH="$BUILD_DIR" "$EXE"
    ;;
  *)
    echo "Unknown OS: $OS_NAME. Try running the exe directly: $EXE"
    "$EXE"
    ;;
esac