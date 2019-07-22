@ECHO OFF	

cpps --clear -c vc -v main.cpp
rem CHECK: compiling main.cpp
rem CHECK-NEXT: compiling foo.cpp
rem CHECK-NEXT: compiling bar.cpp
rem CHECK-NEXT: linking main.cpp.vc.exe

