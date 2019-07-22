#!/usr/bin/env bash

cpps --clear -c gcc -v main.cpp
# CHECK: compiling main.cpp
# CHECK-NEXT: compiling foo.cpp
# CHECK-NEXT: compiling bar.cpp
# CHECK-NEXT: linking main.cpp.gcc.exe
