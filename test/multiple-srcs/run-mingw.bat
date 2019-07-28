@ECHO OFF

DEL main.cpp > NUL 2>&1
COPY backup\main.cpp .
DEL bar.cpp > NUL 2>&1
COPY backup\bar.cpp .


cpps --clear -c mingw -v main.cpp
@REM CHECK: compiling main.cpp
@REM CHECK-NEXT: compiling foo.cpp
@REM CHECK-NEXT: compiling bar.cpp
@REM CHECK-NEXT: linking main.cpp.mingw.exe

DEL bar.cpp > NUL 2>&1
cpps --clear -c mingw -v main.cpp
@REM CHECK: bar.cpp
@REM CHECK-SAME: does NOT exsit

DEL main.cpp > NUL 2>&1
COPY backup\main-nocheck.cpp main.cpp
cpps --clear -c mingw -v main.cpp
@REM CHECK: scanning main.cpp
@REM CHECK-NOT: does NOT exsit
@REM CHECK: compiling main.cpp
