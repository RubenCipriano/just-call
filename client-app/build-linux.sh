#!/bin/bash

mkdir -p build
cd build
cmake .. -G Ninja "-DCMAKE_TOOLCHAIN_FILE=/home/rub1/Documentos/vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build .