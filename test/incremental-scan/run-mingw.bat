@ECHO OFF 

DEL one.* > NUL
copy backup\void-one.cpp .\one.cpp
copy backup\void-one.h .\one.h
DEL three.* > NUL
copy backup\void-three.cpp .\three.cpp
copy backup\void-three.h .\three.h

cpps --clear -c mingw -v main.cpp
REM CHECK: scanning main.cpp
REM CHECK-NEXT: scanning one.cpp
REM CHECK-NEXT: scanning two.cpp
REM CHECK-NEXT: scanning three.cpp
REM CHECK-NEXT: scanning four.cpp
REM CHECK-NEXT: scanning five.cpp

del one.* > NUL
copy backup\int-one.cpp .\one.cpp
copy backup\int-one.h .\one.h
del three.* > NUL
copy backup\int-three.cpp .\three.cpp
copy backup\int-three.h .\three.h

cpps -c mingw -v main.cpp
REM CHECK: scanning one.cpp
REM CHECK-NEXT: scanning three.cpp
