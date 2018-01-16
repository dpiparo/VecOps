#!/bin/bash
set -ex

SRC_DIR=$(dirname "$0")
BUILD_DIR=$(mktemp -d)
cd "${BUILD_DIR}"
cmake "${SRC_DIR}"
cmake --build .
ctest -V
