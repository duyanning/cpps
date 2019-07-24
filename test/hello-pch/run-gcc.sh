#!/usr/bin/env bash

rm main.cpp
cp backup/main-welcome.cpp main.cpp > /dev/null
rm std.h
cp backup/std-a.h std.h > /dev/null

cpps --clear -c gcc  -v main.cpp
# CHECK: precompiling std.h
# CHECK-NEXT: compiling main.cpp
# CHECK-NEXT: compiling std.cpp
# CHECK-NEXT: linking main.cpp.gcc.exe
# CHECK-NEXT: welcome

rm main.cpp
cp backup/main-easy.cpp main.cpp > /dev/null

cpps -c gcc -v main.cpp
# CHECK: compiling main.cpp
# CHECK-NEXT: linking main.cpp.gcc.exe
# CHECK-NEXT: cpps is easy to use


rm std.h
cp backup/std-b.h std.h > /dev/null
cpps -c gcc -v main.cpp
# CHECK: precompiling std.h
# CHECK-NEXT: compiling main.cpp
# CHECK-NEXT: compiling std.cpp
# CHECK-NEXT: cpps is easy to use

rm main.cpp
cp backup/main-welcome.cpp main.cpp > /dev/null
rm std.h
cp backup/std-a.h std.h > /dev/null