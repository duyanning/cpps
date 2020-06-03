@ECHO OFF

cpps --clear -c vc main.cpp
@REM CHECK: hel lo, white space
@REM white space in .d file may cause problem.
cpps -c vc main.cpp
@REM CHECK: hel lo, white space
