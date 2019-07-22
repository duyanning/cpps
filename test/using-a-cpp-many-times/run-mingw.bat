@ECHO OFF	

cpps --clear -c mingw -v main.cpp
rem CHECK: compiling main.cpp
rem CHECK-NEXT: compiling foo.cpp
rem CHECK-NEXT: compiling bar.cpp
rem CHECK-NEXT: linking main.cpp.mingw.exe
