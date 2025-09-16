#!/usr/bin/env bash
set -euo pipefail

# bin/lint.sh — run clang-tidy over C sources and headers using compile_commands.json
# Usage: bin/lint.sh [-p build] [--] [clang-tidy options]

BUILD_DIR=build
# Default to serial; caller can request parallelism with -j N
PARALLEL=1

while [[ $# -gt 0 ]]; do
  case "$1" in
    -p|--build)
      BUILD_DIR="$2"; shift 2;;
    -j)
      PARALLEL="$2"; shift 2;;
    --)
      shift; break;;
    -h|--help)
      echo "Usage: $0 [-p build_dir] [-j jobs] [-- extra clang-tidy args]"; exit 0;;
    *)
      # pass through unrecognized args to clang-tidy
      break;;
  esac
done

if ! command -v clang-tidy >/dev/null 2>&1; then
  echo "clang-tidy not found in PATH" >&2
  exit 2
fi

if [[ ! -f "$BUILD_DIR/compile_commands.json" ]]; then
  echo "compile_commands.json not found in ${BUILD_DIR}. Run: cmake -S . -B ${BUILD_DIR} -DCMAKE_EXPORT_COMPILE_COMMANDS=ON" >&2
  exit 2
fi

# Build file list: .c and .h under src/ and include/
FILES=()
# collect files from find; skip any empty entries (some find implementations
# produce a trailing NUL which would add an empty string to the array)
while IFS= read -r -d $'\0' f; do
  [[ -n "$f" ]] || continue
  FILES+=("$f")
done < <(find src include -type f \( -name '*.c' -o -name '*.h' \) -print0)


if [[ ${#FILES[@]} -eq 0 ]]; then
  echo "No source files found under src/ or include/"; exit 0
fi

CLANG_TIDY_ARGS=("-p" "${BUILD_DIR}")
if [[ $PARALLEL -gt 1 ]]; then
  # run-clang-tidy.py (if available) is better for parallelism
  if command -v run-clang-tidy.py >/dev/null 2>&1; then
    run-clang-tidy.py -p "${BUILD_DIR}" -j "$PARALLEL" "${FILES[@]}" "$@"
    exit $?
  fi
fi

# xargs approach: will call clang-tidy for each file. This may be slow but is compatible.
# Only pass -P to xargs when we actually want parallelism (>1). Some xargs
# implementations treat -P 0 or -P 1 specially; avoid passing -P 0.
if [[ $PARALLEL -gt 1 ]]; then
  # place filename before '--' so clang-tidy receives the file as input
  printf '%s\0' "${FILES[@]}" | xargs -0 -n1 -P ${PARALLEL} -I{} clang-tidy "${CLANG_TIDY_ARGS[@]}" {} -- "$@"
else
  printf '%s\0' "${FILES[@]}" | xargs -0 -n1 -I{} clang-tidy "${CLANG_TIDY_ARGS[@]}" {} -- "$@"
fi
