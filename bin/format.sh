#!/bin/bash

RESET="\033[0m"
BOLD="\033[1m"
GREEN="\033[32m"
YELLOW="\033[33m"

run_clang_format() {
    local file_list="$1"
    for file in $file_list; do
        clang-format -i "$file"
        echo -e $GREEN "\t> $file" $RESET
    done
}

if [ $# -eq 0 ]; then
    files=$(find . -type f \( -name "*.cpp" -o -name "*.hpp" \) ! -path '*/.*' ! -path './build/*')

    if [ -n "$files" ]; then
        echo -e $BOLD$GREEN " >> Running clang-format on all files..." $RESET
        run_clang_format "$files"
        echo -e $BOLD$GREEN " >> clang-format complete for all files!" $RESET
    else
        echo -e $YELLOW "No .cpp or .hpp files found." $RESET
    fi
else
    echo -e $BOLD$GREEN " >> Running clang-format on specified files..." $RESET
    for path in "$@"; do
        if [[ -f $path ]]; then
            case "$path" in
                *.cpp|*.hpp)
                    clang-format -i "$path"
                    echo -e $GREEN "\t> $path" $RESET
                    ;;
                *)
                    echo -e $YELLOW "\t> Skipping $path (not a .cpp or .hpp file)" $RESET
                    ;;
            esac
        else
            echo -e $YELLOW "\t> Skipping $path (file does not exist)" $RESET
        fi
    done
    echo -e $BOLD$GREEN " >> clang-format complete for specified files!" $RESET
fi
