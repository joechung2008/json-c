#!/usr/bin/env bash
# Configure and build the project (POSIX)
set -euo pipefail
ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="$ROOT_DIR/build"
mkdir -p "$BUILD_DIR"

# Determine build type: precedence order
# 1) --type / -t argument
# 2) first positional argument
# 3) environment variable JSON_C_BUILD_TYPE
# 4) default to Debug
_print_usage() {
	cat <<EOF
Usage: $(basename "$0") [OPTIONS] [BUILD_TYPE]

Options:
	-t, --type TYPE   Set CMake build type (Debug or Release). Default: Debug
	-h, --help        Show this help message

You can also set JSON_C_BUILD_TYPE environment variable.
EOF
}

BUILD_TYPE="${JSON_C_BUILD_TYPE:-}"

# Parse flags (simple loop)
while [[ $# -gt 0 ]]; do
	case "$1" in
		-t|--type)
			if [[ -n "${2:-}" ]]; then
				BUILD_TYPE="$2"
				shift 2
				continue
			else
				echo "Error: --type requires an argument" >&2
				_print_usage
				exit 2
			fi
			;;
		-h|--help)
			_print_usage
			exit 0
			;;
		--) shift; break ;; # end of options
		-*)
			echo "Unknown option: $1" >&2
			_print_usage
			exit 2
			;;
		*)
			# first positional argument as build type if not already set
			if [[ -z "$BUILD_TYPE" ]]; then
				BUILD_TYPE="$1"
			fi
			shift
			;;
	esac
done

# Default to Debug if not provided
if [[ -z "$BUILD_TYPE" ]]; then
	BUILD_TYPE="Debug"
fi

# Normalize and validate
case "${BUILD_TYPE,,}" in
	debug)
		BUILD_TYPE="Debug" ;;
	release)
		BUILD_TYPE="Release" ;;
	*)
		echo "Invalid build type: '$BUILD_TYPE'. Allowed: Debug or Release." >&2
		_print_usage
		exit 2
		;;
esac

echo "Configuring build (type: $BUILD_TYPE)"
cmake -S "$ROOT_DIR" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -DCMAKE_C_FLAGS="-Wall" -Wno-dev
cmake --build "$BUILD_DIR" --parallel

echo "Build complete. Build artifacts are in: $BUILD_DIR"