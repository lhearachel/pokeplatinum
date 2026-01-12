#!/usr/bin/env bash

set -euo pipefail

usage() {
    echo "Usage: ${0##*/} [options] NAME"
    echo ""
    echo "Options:"
    echo "  --manifest-path FILE  Use FILE as the Cargo manifest (Cargo.toml)"
    echo "  --target-dir DIR      Output build-profile artifacts to DIR"
}

while [ "$#" -gt 1 ]; do
    case "${1}" in
        --manifest-path) CARGO_MANIFEST_PATH="${2}"; shift; shift;;
        --target-dir) CARGO_TARGET_DIR="${2}"; shift; shift;;
        --help) usage; exit 0;;
        *) usage; exit 1;;
    esac
done

if [[ "$#" -eq 0 ]]; then usage; exit 1; fi

CARGO_BIN_NAME="${1}"
cargo rustc --manifest-path="${CARGO_MANIFEST_PATH}" \
    --target-dir="${CARGO_TARGET_DIR}" \
    --bin="${CARGO_BIN_NAME}" \
    --quiet

cp "debug/${CARGO_BIN_NAME}" "${CARGO_TARGET_DIR}"
