@ECHO OFF

cpps --clear -c mingw main.cpp
@REM CHECK: hel lo, white space
@REM white space in .d file may cause problem.
cpps -c mingw main.cpp
@REM CHECK: hel lo, white space
