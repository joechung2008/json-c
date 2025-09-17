#!/usr/bin/env bash
set -euo pipefail

# Minimal lint helper for clang-tidy. Opinionated and simple.
# Usage: bin/lint.sh

BUILD_DIR=build
JOBS=0

if [[ $# -ne 0 ]]; then
  echo "lint.sh takes no arguments. If you want to run clang-tidy manually, call clang-tidy directly." >&2
  exit 2
fi

if ! command -v clang-tidy >/dev/null 2>&1; then
  echo "clang-tidy not found in PATH" >&2
  exit 2
fi

if [[ ! -f "$BUILD_DIR/compile_commands.json" ]]; then
  echo "compile_commands.json not found in ${BUILD_DIR}. Run: cmake -S . -B ${BUILD_DIR} -DCMAKE_EXPORT_COMPILE_COMMANDS=ON" >&2
  exit 2
fi

# Build file list: .c and .h under src/ and include/
mapfile -d $'\0' FILES < <(find src include -type f \( -name '*.c' -o -name '*.h' \) -print0)

if [[ ${#FILES[@]} -eq 0 ]]; then
  echo "No source files found under src/ or include/"; exit 0
fi

# Default header-filter: show all non-system headers (avoid clang-tidy hint)
DEFAULT_HEADER_FILTER='-header-filter=.*'

# We intentionally don't support parallelism or extra args here. Keep behavior
# simple and reproducible: run clang-tidy per file with a conservative
# header-filter so system headers are ignored by default.

# Run clang-tidy serially (simple, predictable). This script intentionally
# does not accept extra args; to override behavior, invoke clang-tidy yourself.
CLANG_ARGS=("-p" "$BUILD_DIR" "$DEFAULT_HEADER_FILTER")
overall_rc=0
for f in "${FILES[@]}"; do
  tmpf=$(mktemp)
  clang-tidy "${CLANG_ARGS[@]}" "$f" &> "$tmpf"
  rc=$?
  # Filter noisy summary/hint lines that clang-tidy prints when it suppresses diagnostics.
  # Use grep -v with a single robust regex to avoid issues with line wrapping and sed dialects.
  grep -v -E "^[[:space:]]*Suppressed [0-9]+ warnings|header-filter|system-headers|[0-9]+ warning[s]?( and [0-9]+ errors)? generated\.?$" "$tmpf" || true
  rm -f "$tmpf"
  if [[ $rc -ne 0 ]]; then overall_rc=$rc; fi
done

if [[ $overall_rc -ne 0 ]]; then
  exit $overall_rc
fi

# Exit with success (clang-tidy results are printed; use exit codes externally if needed)
exit 0
