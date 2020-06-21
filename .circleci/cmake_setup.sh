#!/bin/bash

## Perform the cmake setup with the proper flags turned on for the given
## CI run.
set -euo pipefail


## ----------------------------------------------------------------------------
## Constants & Utility Functions

RED='\033[0;31m'
GREEN='\033[0;32m'
CLEAR='\033[0m'

function fatal() { local msg=$1;
    echo -e "${RED}[FATAL]${CLEAR} ${msg}" >&2
    exit 1
}
function info() { local msg=$1;
    echo -e "${GREEN}[INFO]${CLEAR} ${msg}"
}


## ----------------------------------------------------------------------------
## Command Line Parsing

SANITIZER_FLAGS=''
COMPILATION_FLAGS=''
CLANG_TIDY_FLAGS=''

function set_sanitizer_flags() { local sanitizer=$1;
    case "$sanitizer" in
        asan)
            info "Running test with ASAN support"
            SANITIZER_FLAGS="${SANITIZER_FLAGS} -DENABLE_ASAN=ON"
            ;;
        tsan)
            info "Running test with TSAN support"
            SANITIZER_FLAGS="${SANITIZER_FLAGS} -DENABLE_TSAN=ON"
            ;;
        ubsan)
            info "Running test with UBSAN support"
            SANITIZER_FLAGS="${SANITIZER_FLAGS} -DENABLE_UBSAN=ON"
            ;;
        none)
            info "Running without sanitizers"
            SANITIZER_FLAGS=''
            ;;
        *)
            fatal "Unsupported sanitizer value provided '${sanitizer}'"
            ;;
    esac
}

function set_compilation_mode_flags() { local mode=$1;
    case "$mode" in
        Debug|debug)
            info "Compiling in Debug mode"
            COMPILATION_FLAGS='-DCMAKE_BUILD_TYPE=Debug'
            ;;
        Rel*|rel*)
            info "Compiling in Release mode"
            COMPILATION_FLAGS='-DCMAKE_BUILD_TYPE=RelWithDebInfo'
            ;;
    esac
}

function set_clang_tidy_flags() { local check=$1;
    local tidy;
    tidy=$(which clang-tidy-10)
    CLANG_TIDY_FLAGS="-DCMAKE_CXX_CLANG_TIDY=${tidy};-checks=${check}-*;--warnings-as-errors=${check}-*"
}

function print_help() {
    cat <<EOF
$(basename $0) [options]

Setup CMake for CircleCI builds

OPTIONS
    -s | --with-sanitizer    Set sanitizer to one of [asan, tsan, msan, ubsan]
    -c | --compilation-mode  Set compilation mode to one of [debug, release]
    -t | --with-clang-tidy   Set clang-tidy check prefix (will not run tidy if
                             flag is not provided)
    -h | --help              Show this help-message

EOF
}

while (( "$#" )); do
    case "$1" in
        -s|--with-sanitizer)
            [[ -z "$2" ]] && fatal "Must provide argument for '--with-sanitizer' flag"
            set_sanitizer_flags "$2"
            shift 2
            ;;
        -c|--compilation-mode)
            [[ -z "$2" ]] && fatal "Must provide argument for '--compilation-mode' flag"
            set_compilation_mode_flags "$2"
            shift 2
            ;;
        -t|--with-clang-tidy)
            [[ -z "$2" ]] && fatal "Must provide argument for '--with-clang-tidy' flag"
            set_clang_tidy_flags "$2"
            shift 2
            ;;
        -h|--help|help)
            print_help
            exit 1
            ;;
        *) # unsupported flags
            print_help
            fatal "Error: Unsupported flag $1"
            ;;
    esac
done



## ----------------------------------------------------------------------------
## Setup CMake

[[ -d build ]] || fatal "'build' directory seems to be missing"
cd build

# shellcheck disable=SC2086
env CC=/usr/bin/clang-10 CXX=/usr/bin/clang++-10 cmake \
    $COMPILATION_FLAGS                                \
    $SANITIZER_FLAGS                                  \
    $CLANG_TIDY_FLAGS                                  \
    ..