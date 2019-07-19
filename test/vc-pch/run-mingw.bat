@ECHO OFF	

DEL main.cpp
COPY backup\main-welcome.cpp main.cpp > NUL
DEL std.h
COPY backup\std-a.h std.h > NUL

cpps --clear -c mingw  -v main.cpp
rem CHECK: precompiling std.h
rem CHECK-NEXT: compiling main.cpp
rem CHECK-NEXT: compiling std.cpp
rem CHECK-NEXT: linking main.cpp.mingw.exe
rem CHECK-NEXT: welcome

DEL main.cpp
COPY backup\main-easy.cpp main.cpp > NUL

cpps -c mingw -v main.cpp
rem CHECK-NEXT: compiling main.cpp
rem CHECK-NEXT: linking main.cpp.mingw.exe
rem CHECK-NEXT: cpps is easy to use


DEL std.h
COPY backup\std-b.h std.h > NUL
cpps -c mingw -v main.cpp
REM CHECK-NEXT: precompiling std.h
rem CHECK-NEXT: compiling main.cpp
rem CHECK-NEXT: compiling std.cpp
rem CHECK-NEXT: cpps is easy to use

DEL main.cpp
COPY backup\main-welcome.cpp main.cpp > NUL
DEL std.h
COPY backup\std-a.h std.h > NUL
