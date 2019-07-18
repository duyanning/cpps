#!/usr/bin/env bash

cpps -v --clear -c gcc main.cpp
# CHECK: precompiling std.h
# CHECK: compiling main.cpp
# CHECK-NEXT: linking main.cpp.gcc.exe
# CHECK-NEXT: welcome to cpps


mv std.h global.h
cpps -v -c gcc main.cpp 2>&1
# CHECK: std.h
# CHECK-SAME: does NOT exsit

mv global.h std.h
