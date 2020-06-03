#!/usr/bin/env bash

cpps --clear -c gcc main.cpp
# CHECK: hel lo, white space
# white space in .d file may cause problem.
cpps -c gcc main.cpp
# CHECK: hel lo, white space
