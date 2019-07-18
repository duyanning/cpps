#!/usr/bin/env bash

cpps -v --clear -c gcc main.cpp
# CHECK: compiling main.cpp
# CHECK-NEXT: linking main.cpp.gcc.exe
# CHECK-NEXT: welcome to cpps

mv foo.h bar.h
cpps -v -c gcc main.cpp 2>&1
# CHECK: compiling main.cpp
# CHECK-NEXT: foo.h: No such file

mv bar.h foo.h
