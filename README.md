# CPPS - A (Pseudo) Interpreter for C++ Script

Note: There is also [a Chinese version](https://github.com/duyanning/cpps/blob/master/README.chinese.md).

## Build & Install

    mkdir build-cpps
    cd build-cpps
    cmake ../cpps
    make
    make test
    sudo make install

## Script consisting of only one .cpp file

    cpps hello.cpp

## Script consisting of multiple one .cpp files

Such as:
* hello.cpp
* foo.cpp
* foo.h

### If you use a traditional approach such as GCC & Makefile

You need add a line such as

    #include "foo.h"

to `hello.cpp` that calls functions defined in `foo.cpp` and declared in `foo.h`
and then add rules for compiling `foo.cpp` to your Makefile.

### If you use CPPS

What you need is to add a comment after the `#include "foo.h"` directive, such as

    #include "foo.h" // usingcpp
    
and then run the following command:

    cpps hello.cpp

NOTE: if your .cpp file has a different name than the corresponding .h file, you should write:

    #include "foo.h" // using fooandbar.cpp

where `fooandbar.cpp` is the file in which the definations of functions declared in foo.h reside.

## If you use a library in hello.cpp
such as pthread

### If you use a traditional approach such as GCC & Makefile

You need write

    g++ -lpthread hello.cpp -o hello

### If you ues CPPS

All you need to do is adding the following comment to any .cpp file of your script

    // linklib pthread

## Request a .h to be precompiled
For example, if you want to make `std.h` a precompiled header,
all you need to do is adding a comment after the `#include "std.h"` directive as following:

    #include "std.h" // precompile

