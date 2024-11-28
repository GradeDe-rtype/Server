#!/bin/bash

# Script to clean up CMake build files
BUILD_DIR="build"
LIB_DIR="lib"
INCLUDE_DIR="include/lib"
EXECUTABLE="r-type_server"

RESET="\033[0m"
BOLD="\033[1m"
GREEN="\033[32m"
YELLOW="\033[33m"

echo -e $BOLD$GREEN " >> Cleaning up CMake build artifacts..." $RESET

# Remove the build directory
if [ -d "$BUILD_DIR" ]; then
    echo -e $GREEN "\t> Removing build directory..." $RESET
    rm -rf "$BUILD_DIR"
else
    echo -e $YELLOW "\t> Build directory not found, skipping..." $RESET
fi

# Remove the compiled library
if [ -d "$LIB_DIR" ]; then
    echo -e $GREEN "\t> Cleaning libraries in $LIB_DIR..." $RESET
    find "$LIB_DIR" -type f -name "*.a" -exec rm -f {} \;
else
    echo -e $YELLOW "\t> Library directory not found, skipping..." $RESET
fi

# Remove the main executable
if [ -f "$EXECUTABLE" ]; then
    echo -e $GREEN "\t> Removing executable $EXECUTABLE..." $RESET
    rm -f "$EXECUTABLE"
else
    echo -e $YELLOW "\t> Executable not found, skipping..." $RESET
fi

echo -e $GREEN$BOLD " >> Clean-up complete!" $RESET
