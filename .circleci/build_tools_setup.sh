#!/bin/bash
set -euxo pipefail

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

## ----------------------------------------------------------------------------
## Install dependencies
export DEBIAN_FRONTEND=noninteractive
apt-get install -y --no-install-recommends \
    build-essential  \
    clang-10         \
    clang-tidy-10    \
    doxygen          \
    libc++-10-dev    \
    curl             \
    ninja-build      \
    python3          \
    python3-pip

pip3 install -U pip
pip3 install cmake conan
pip3 install -r "${DIR}/../docs/requirements.txt"

# Configure conan
mkdir -p "$HOME/.conan/profiles"
cp "${DIR}/clang.conan_profile" "$HOME/.conan/profiles/default"
conan config get