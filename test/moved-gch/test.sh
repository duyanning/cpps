#!/usr/bin/env bash

cpps -v --clear main.cpp
# CHECK: precompiling std.h
# CHECK: compiling main.cpp
# CHECK-NEXT: linking main.cpp.exe
# CHECK-NEXT: welcome to cpps


mv std.h global.h
cpps -v main.cpp 2>&1
# CHECK: std.h
# CHECK-SAME: does NOT exsit

mv global.h std.h
