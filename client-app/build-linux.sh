#!/bin/bash

mkdir -p build
cd build
cmake .. "-DCMAKE_TOOLCHAIN_FILE=/caminho/para/vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build .