#!/bin/bash

set -uexo pipefail

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

# Configure conan
mkdir -p "$HOME/.conan/profiles"
cp "${DIR}/clang.conan_profile" "$HOME/.conan/profiles/default"
conan config get
      
# Fetch 3rd party deps
mkdir -p build
cd build
conan install ../pembroke --build=missing --env CC=/usr/bin/clang-10 --env CXX=/usr/bin/clang++-10