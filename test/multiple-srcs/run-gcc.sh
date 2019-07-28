#!/usr/bin/env bash

rm main.cpp > /dev/null 2>&1
cp backup/main.cpp .
rm bar.cpp > /dev/null 2>&1
cp backup/bar.cpp .


cpps --clear -c gcc -v main.cpp
# CHECK: compiling main.cpp
# CHECK-NEXT: compiling foo.cpp
# CHECK-NEXT: compiling bar.cpp
# CHECK-NEXT: linking main.cpp.gcc.exe

rm bar.cpp > /dev/null 2>&1
cpps --clear -c gcc -v main.cpp
# CHECK: bar.cpp
# CHECK-SAME: does NOT exsit

rm main.cpp > /dev/null 2>&1
cp backup/main-nocheck.cpp main.cpp
cpps --clear -c gcc -v main.cpp
# CHECK: scanning main.cpp
# CHECK-NOT: does NOT exsit
# CHECK: compiling main.cpp
