#!/usr/bin/env bash

touch -t 201102100610 hello.cpp
cpps -v --clear hello.cpp
# CHECK: compiling hello.cpp
# CHECK-NEXT: linking hello.cpp.exe
# CHECK-NEXT: hello, world

cpps -v hello.cpp
# CHECK-NOT: compiling hello.cpp
# CHECK-NEXT: hello, world

touch -t 197801030000 hello.cpp
cpps -v hello.cpp

# 用哈希值的话，就不会有底下这行，因为哈希值没变
# compiling hello.cpp

# 并不一定重新生成exe，因为obj虽然重新生成了，但是obj的签名可能并没变(签名：时间+大小)
# linking hello.cpp.exe

# CHECK-NEXT: hello, world
