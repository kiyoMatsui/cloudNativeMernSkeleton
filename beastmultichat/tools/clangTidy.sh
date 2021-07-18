#!/bin/bash
myDir=$PWD
clang-tidy ./src/main.cpp -p ./build -header-filter=.* -- -I${PWD}/src

