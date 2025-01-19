#!/bin/bash

RESET="\033[0m"
BOLD="\033[1m"
GREEN="\033[32m"

TESTS_DIR="$(dirname "$(realpath "$0")")/../tests"
BUILD_DIR="$TESTS_DIR/build"

echo -e $BOLD$GREEN " >> Cleaning previous build..." $RESET
rm -rf "$BUILD_DIR"

echo -e $BOLD$GREEN " >> Creating build directory..." $RESET
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR" || exit 1

echo -e $BOLD$GREEN " >> Configuring with CMake..." $RESET
cmake ..

echo -e $BOLD$GREEN " >> Compiling tests..." $RESET
make r-type_tests

if [ $? -eq 0 ]; then
    echo -e $BOLD$GREEN " >> Running tests..." $RESET
    ./r-type_tests
else
    echo -e $BOLD$GREEN " >> Error during test compilation." $RESET
    exit 1
fi
