#!/bin/bash
rm -rf build/rtagsDir
rdm &
mkdir build
cd build
mkdir rtagsDir
cd rtagsDir
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ../../
rc -J
