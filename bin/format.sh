#!/usr/bin/env bash

# Formats tracked C and header files using clang-format.
# Prefer git tracked files; fall back to find if git not available or not a repo.

set -euo pipefail

CLANG_FORMAT=${CLANG_FORMAT:-clang-format}

# Use git if possible to get tracked files (handles submodules, ignores build artifacts)
if command -v git >/dev/null 2>&1 && git rev-parse --is-inside-work-tree >/dev/null 2>&1; then
    git ls-files -z '*.c' '*.h' | xargs -0 --no-run-if-empty $CLANG_FORMAT -i
else
    # Fallback: find all .c/.h files in the repo root excluding build
    find . -path ./build -prune -o -name '*.c' -o -name '*.h' -print0 | xargs -0 --no-run-if-empty $CLANG_FORMAT -i
fi

echo "Formatting complete."