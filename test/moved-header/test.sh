#!/usr/bin/env bash

cpps -v --clear main.cpp
# CHECK: compiling main.cpp
# CHECK-NEXT: linking main.cpp.exe
# CHECK-NEXT: welcome to cpps

mv foo.h bar.h
cpps -v main.cpp 2>&1
# CHECK: compiling main.cpp
# CHECK-NEXT: foo.h: No such file

mv bar.h foo.h
