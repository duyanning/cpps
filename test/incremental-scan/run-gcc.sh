#!/usr/bin/env bash

rm one.* > /dev/null
cp backup/void-one.cpp ./one.cpp
cp backup/void-one.h ./one.h
rm three.* > /dev/null
cp backup/void-three.cpp ./three.cpp
cp backup/void-three.h ./three.h

cpps --clear -c gcc -v main.cpp
# CHECK: scanning main.cpp
# CHECK-NEXT: scanning one.cpp
# CHECK-NEXT: scanning two.cpp
# CHECK-NEXT: scanning three.cpp
# CHECK-NEXT: scanning four.cpp
# CHECK-NEXT: scanning five.cpp

rm one.* > /dev/null
cp backup/int-one.cpp ./one.cpp
cp backup/int-one.h ./one.h
rm three.* > /dev/null
cp backup/int-three.cpp ./three.cpp
cp backup/int-three.h ./three.h

cpps -c gcc -v main.cpp
# CHECK: scanning one.cpp
# CHECK-NEXT: scanning three.cpp
