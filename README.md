[![中文](https://img.shields.io/badge/语言-中文-red.svg)](https://github.com/duyanning/cpps/blob/master/README.zh.md)

#                                                                                                                                 Introduction                                

Apparently, `cpps` is an interpreter for C++ scripts which can execute `.cpp` files directly just like `python` executes `.py` files directly.

In essence, `cpps` is actually a C++ project building system engine that does not require any *Makefile*. It calls the underlying compilers (*GCC*, *MinGW*, *Visual C++*, *Clang*) to compile `.cpp` files into `.exe` files, and then executes the resulting `.exe` files, but does not leave `.o` files or `.exe` files in the directory.

# Basic usage  

## If the script consists of only one .cpp file  
For example, you have a hello.cpp  
```C++
#include <iostream>

using namespace std;

int main()
{
    cout << "hello, world" << endl;
}
```

Then you execute the following line:
```shell
$ cpps hello.cpp
```

## If the script consists of multiple .cpp files  
For example, your c++ code is located in the directory `~/work/hello`, and there are three files: `main.cpp`, `foo.cpp`, and `foo.h`, with the following contents:

```C++
// main.cpp
#include <iostream>
#include "foo.h"

using namespace std;

int main()
{
    cout << "hello, the result is " << foo() << endl;
}

```

```C++
// foo.cpp
#include "foo.h"
int foo()
{
    return 5;
}
```

```C++
// foo.h
#ifndef FOO_H
#define FOO_H
int foo();
#endif
```

If you use the *make*+*Makefile* scheme, in addition to the source files, you will need to prepare a *Makefile* with the following contents:

```Makefile
hello: main.o foo.o
    g++ -o hello main.o foo.o

main.o: main.cpp foo.h
    g++ -c main.cpp

foo.o: foo.cpp foo.h
    g++ -c foo.cpp
```

Then execute:

```shell
$ make
$ ./hello
```

But if you use `cpps`, just find the following line in `main.cpp`  

```C++
#include "foo.h"
```
and add a comment at the end of this line so that it reads:

```C++
#include "foo.h" // usingcpp
```

Then execute:

```shell
$ cpps main.cpp
```
In addition to the `usingcpp` directive, you can also use the `using` directive, which is equivalent to the above:

```c++
#include "foo.h" // using foo.cpp
```
This style is mainly used when the `.cpp` file has a different base name from the `.h` file.  


## Link a library  
If you use a library, such as `pthread`, in `hello.cpp`, then, in the `gcc` traditional compilation environment, you need to write `-lpthread` on the command line of `gcc` to link the library
in. Whereas in the interpretation environment of `cpps`, you can simply add a comment like this to any `.cpp` file of the program.  

```C++
// linklib pthread
```
The recommended practice is to write the `linklib` directive in whichever `.cpp` file uses the library. If multiple `.cpp` files use the same library, it's okay to write it multiple times. I think it's better to write it centrally in the `.cpp` file where the `main` function is located.  


# Philosophy  

## Scripts vs. Programs  
Let's start with the difference (in my opinion) between a program and a script.  

The **program** is independent of the data you want to process: the program has its own directory and can process different data located in different locations.

The **script** is dependent on the data it is working with: the script is thrown in the directory where the data is located, the script refers directly to a specific file or directory in the data, and it will not run outside of that environment.

In addition to this, scripting has the feature that it has only two stages: *write* and *run*.

And there are usually three stages to a program: *write*, *build*, and *run*.

And the executable file generated in the *build* stage is not located where the data to be processed is located. It's hard to distribute it along with the data.

The build stage also generates a bunch of intermediate files that mess up the directory where the data is located.

In a nutshell, a script is all about: **write in place, run in place**.

This is exactly what `cpps` is after.  

## Why you need C++ scripts  
Because you use C++ the most, because you don't know/are not familiar with/want to learn other scripting languages.  

Early scripting languages were relatively simple (e.g., batch files for *dos*, shell scripts for *unix*), and it was difficult to write serious programs.

Nowadays, scripting languages are becoming more and more powerful (e.g. perl, python) and can be used to write serious programs, so can't an already powerful programming language (e.g. C++) be lowered to write scripts?

Of course you can!  

## Why is cpps a pseudo-interpreter?  
This is because it is not an interpreter in the traditional sense, but rather an underlying compiler (such as `gcc`) is called behind the scenes to compile the source code and then execute the resulting exe program.

## Advantages of cpps over true interpreters  
* Support all C++ syntax supported by the underlying compiler (e.g. gcc) (C++ source code is compiled by the underlying compiler and all `cpps` instructions are in the form of C++ comments)
* Fast script execution (execution of machine language code generated by the underlying compiler)

## Disadvantages of cpps over true interpreters  
* No *REPL*, cannot be executed interactively
* Cannot be embedded independently of other programs (unless you don't mind your own programs also relying on the underlying compiler)  

## Anything else you might care about
* Won't mess up your working directory (won't leave .o files, executable files in your working directory)
* Supports projects with multiple `.cpp` files and does not require any type of project files (*Makefile*, *CMakeLists.txt* or any other stream, only `.cpp` and `.h`)
* Intelligently decide which `.cpp` need to be recompiled
* Automatically generate, use, and manage *precompiled header* files
* Can link libraries
* Support *shebang*

## How did it all work?
Since C++ code takes a long time to compile, it is not acceptable to compile it before each *interpretation*. Therefore, `cpps` caches the `.o` files and `.exe` files generated by the previous run. (cache is located in `~/.cpps/cache`)

Executing C++ scripts with `cpps` goes through the following stages: *scan* > *build* > *run*

where *build* is the familiar compilation and linking process.

*scan* means that `cpps` scans through the contents of the `.cpp` file and extracts the `cpps` directives from it before calling the underlying compiler to compile it.

Some of these directives are used to instruct `cpps` which `.cpp` files to include in the project, some are used to mark headers that need to be pre-compiled, and some are used to tell cpps which libraries need to be linked and which compile or link switches to use. 

Then `cpps` builds a dependency graph in memory that reflects the dependencies between `.exe` files, `.obj` files, `.cpp` files, and `.h` files. When you run the script for the first time, all `.cpp` will be compiled; but if you make another change to the code, only the changed `.cpp` files will be recompiled when you run the script again. Provided, of course, that your script is composed of multiple `.cpp` files (the smaller the `.cpp` file, the more obvious the advantage). 

When running `cpps`, you can try adding the `-v` parameter to see if `cpps` compiles files that don't need to be compiled. This is not only true for the *build* stage, but also for the *scan* stage, where only changed `.cpp` files are rescanned.


## Philosophy of cpps  

Any build system consists of two parts:

- A build system engine (e.g. make)
- Some files written by the user (e.g. Makefile)

Generally speaking, the more the *engine* does, the less parts need to be written by the user.

Therefore, `cpps` is not so much an interpreter as a high-level build system engine that requires little information from the user in the form of interpreter directives in `.cpp` files to build a complete build system.
For the `cpps` build system engine, the *Makefile* is stored in `.cpp` files.

Do you need to write *Makefiles* before executing `python` scripts and need to `make` them manually? No need!

So you should not write the *Makefile* by hand and `make` it manually before executing the C++ script.

The `usingcpp` directive in `cpps` is used in conjunction with `#include`, and together they work similarly to `import` in scripting languages like `python`.

C++ is carrying the burden of the old toolchain of the C era. Java's `.class` file is equivalent to `.h` and `.obj` in one, and python's `.pyc` is equivalent to `.h` and `.obj` in one. `.java` and `.py` is just a high-level language form of `.class` and `.pyc`, which is equivalent to `.h` and `.cpp` two in one.  

# Installation  
## Acquire
### GNU/Linux
```
git clone https://github.com/duyanning/cpps.git
```
### Windows
Compiled binaries are provided.

Download, unzip and get a folder which contains `cpps.exe` with several other `.exe`'s.

Then the directory can be added to the environment variable `PATH`.

(The binaries provided, as you can see from the name of the zip, are compiled with *vc* and compiled with *mingw*. This just means that the `cpps` binaries are compiled with *vc* or *mingw*, it doesn't mean which underlying compiler is used for `cpps` execution, which underlying compiler is to be specified in the configuration file or command line when executing `cpps`)  

## Pre-installation preparation  
Take ubuntu as an example  
```ShellSession
sudo apt-get install libboost-filesystem-dev libboost-program-options-dev libboost-serialization-dev libboost-system-dev libboost-regex-dev libboost-timer-dev libboost-chrono-dev
```

## Compile and install  
```ShellSession
mkdir build-cpps
cd build-cpps
cmake ../cpps
make
make test
```

If there is no problem  
```ShellSession
sudo make install
```

I compiled it myself with (note the version number).

- gcc 4.9.1
- boost 1.57.0

Compilation or linking problems may be encountered with lower versions.  

# Usage

The most basic usage has been introduced in the *Introduction* section, so I will not repeat it here, but only some advanced usage.  

## Pre-compiled headers  
You can also speed up the compilation process of cpps by pre-compiling the headers.

For example, if you intend to make `std.h` a pre-compiled header file, then just add a comment like this to one of the `.cpp` files including `std.h`:

```c++
#include "std.h" // precompile
```

Pre-compiled headers are very effective in reducing the compilation time of C++ programs. Highly recommended!

And C++ scripts in the same directory can share the same pre-compiled headers.

I usually get a *std.h*, which includes all the standard library, boost library header files.

With `cpps` you can execute `cpps -g hello.cpp`, which will generate two files, `std.h` and `std.cpp`, in addition to `hello.cpp`.  

## Also support shebang  
In the first line of your `hello.cpp` file, write:
```c++
#!/usr/bin/env cpps
```
Then execute  
```ShellSession
chmod +x hello.cpp
./hello.cpp
```
Does it feel good?  

It's just a shame that this line is not legitimate C++ code.

This will cause you to get an error when you compile hello.cpp with the compiler (although `cpps` does some magic to make its underlying compiler not report an error when compiling).

A better approach would be: instead of adding shebang directly to the .cpp file, create a new file with a `.cpps` extension, such as `hello.cpps`.

Then write shebang in the first line of this file, and give the path to the `.cpp` script in the second line (absolute or relative paths are fine), as follows:

```shell
#!/usr/bin/env cpps
hello.cpp
```

Then set this `.cpps` file to be executable  

```shell
$ chmod +x hello.cpps
$ ./hello.cpps
```
Huh? It seems that calling `cpps hello.cpp` in a shell script can achieve the same effect?

Not exactly, using a shell script will execute the shell one more time, and it won't remind you that it's a C++ script by its extension.

## Clean running  
```shell
$ cpps --clear hello.cpp
```
It is equivalent to first delete the previously generated `.o` file, `.exe` file, and then build from scratch and then run it again.

If you have problems running a previously run C++ program with a new version of `cpps`, try this.

If there are still problems, just remove the cache directory `.cpps/cache` directly.  


## I want to get the executable file that cpps generates behind the scenes  
```shell
$ cpps --build hello.cpp -o hello
```
cpps will then give you a copy of the executable file it generated.  

## Use MingGW as the underlying compiler
MingW needs to be installed and make sure C`:\MinGW\bin` is in your `PATH` environment variable.

If you want to use the *boost* library, you can download someone else's compiled one from [here](https://sourceforge.net/projects/boostmingw/) (since *boost* does not provide official support for *mingw*).

For example, we put the downloaded *boost* under `D:\libs4mingw`.

Then, when executing the program with `cpps`, you also need to specify on the command line the directory where the *boost* header file (`.h`) is located, the directory where the library file (`.a`) is located, and the directory where the dynamic library file (`.dll`) is located, as follows:

```shell
$ cpps -c mingw hello.cpp -ID:\libs4mingw\boost\include -LD:\libs4mingw\boost\lib --dll-dir=D:\libs4mingw\boost\lib
```
Too tedious to specify every time?

You can create a file named config.txt in the `C:\Users\< Your Name >\.cpps` directory and write in it:

```INI
[mingw]
include-dir = D:\libs4mingw\boost\include
lib-dir = D:\libs4mingw\boost\lib
dll-dir = D:\libs4mingw\boost\lib
```
This way you can later just run:

```shell
$ cpps hello.cpp
```
The `include-dir`, `lib-dir` and `dll-dir` in config.txt can appear in multiple lines.

If you have trouble creating a directory with a dot in its name in Explorer, you can simply execute the `.cpp` file once with `cpps` to create the directory.  

## Using Visual C++ as the underlying compiler  
Make sure `minised.exe` and `finderror.exe` are put together with `cpps.exe` (these two are provided with `cpps`).  

```shell
> cpps -c vc main.cpp
```
Use the `-c` parameter to specify the use of *vc*, as above.

Because the options provided by *vc* are not as rich as *gcc*, `cpps` control of *vc* can not be done by ready-made options, but also need to analyze the output of *vc*.

During the analysis, `cpps` will call the `minised.exe` provided by the *gnuwin32* project (which is right under the `minised` folder, you should put it together with `cpps.exe`).

Anyone familiar with the *Visual C++* command line compiler tool knows that you have to run `vcvars32.bat` provided by Visual C++ to set the necessary environment variables before you can execute `cl`.

If you don't want to run `vcvars32.bat` in a `cmd` window every time before running cpps, you can do this:

- Open a new clean `cmd` window
- Run vcvars32.bat in this window to set the environment variables
- In this window run `vc-config-gen.exe` provided by *cpps*
- Then just copy and paste the output into the `vc` block of the config file `config.txt`.  

Once this is done, you can run `cpps` directly without having to execute `vcvars32.bat` first.

Note: After each Visual Studio upgrade, the path where *vc* is located may change. In this case, you need to re-run `vc-config-gen.exe` and update the outdated part of `config.txt` with the output it gives.  

## Summary of interpreter directives  

The vast majority of `cpps` directives exist as C++ single-line comments. Very few can also exist as C++ multi-line comments.

All instructions are written in the `.cpp` file. It is useless to write in `.h`, because `cpps` will not scan the contents of the `.h` file at all.

### `usingcpp` and `using`
It has been introduced in the introduction and will not be repeated.  

### `linklib` and `<compiler>-linklib`
For example, an application that uses the *FLTK* library can link *fltk* in the following form.  

```C++
#include <FL/Fl.H> // linklib fltk
```
If the same library, under different compilers, has different names, you can use the form `<compiler>-linklib` instead of `linklib`. Where `<compiler>` can be `gcc`, `vc`, `mingw`, `clang`, etc.

The library is followed by an extension, which you can add or not.

Wildcards are also supported (the supported wildcards are shell related).  

### `<compiler>-extra-compile-flags`与`<compiler>-extra-link-flags`
Some programs require special compile/link options, for example, if you compile a program using FLTK under Windows with MinGW, in addition to

```C++
// linklib fltk
```

You also need to add the following directive to the .cpp file  

```C++
// mingw-extra-compile-flags: -DWIN32
// mingw-extra-link-flags: -mwindows -lole32 -luuid -lcomctl32
```

### `<compiler>-extra-compile-flags(local)`
This directive, with a `(local)` at the end, differs from `<compiler>-extra-compile-flags` in that the former only works on the current source file, while the latter works on all source files.  

### `cpps-make`
In a realistic C++ project, the source code will involve some other types of files besides the C++ `.cpp` and `.h` files.

After these files are compiled, `.cpp`  or `.h` files are generated, and these generated `.cpp` or `.h` files need to be compiled in.

For example, the *FLTK* interface description file `.fl` is compiled by `fluid` to produce a `.cpp` file and a `.h` file.

Another example is *Qt*'s interface description file `.ui`, which is compiled by `uic` to produce an `.h` file.

The `.h` file containing Q_OBJECT is compiled by `moc` to produce a `.cpp` file.

The resource file `.qrc` is compiled by `rcc` to produce a `.cpp` file.

For this kaleidoscopic stuff, `cpps` provides *embedded makefile*s.

```C++
// using nocheck finddialog.h.cpp
/* cpps-make
finddialog.h.cpp : finddialog.h
	moc finddialog.h -o finddialog.h.cpp
*/

// cpps-make ui_propertiesdialog.h : propertiesdialog.ui // uic propertiesdialog.ui -o ui_propertiesdialog.h

// using nocheck $(SHADOWDIR)/resources.qrc.cpp
// cpps-make $(SHADOWDIR)/resources.qrc.cpp : resources.qrc // rcc resources.qrc -o $(SHADOWDIR)/resources.qrc.cpp

// include-dir $(SHADOWDIR)
```

The `nocheck` after the `using` directive tells the `cpps` that the file called `finddialog.h.cpp` was generated in the *build* stage, so don't report an error even if it doesn't exist in the *scan* stage.

The `cpps-make` directive introduces a user-defined rule with the same syntax as a *Makefile*, but of course, not as complete as a normal *Makefile*. 

cpps-make uses a multi-line cpps-make syntax, much like the one we are
familiar with.

The rule for `moc` uses the multiline `cpp-make` syntax, much like the familiar syntax.

The two rules for `uic` and `rcc` use a single line `cpps-make` syntax.

That `rcc` rule also uses the variable `$(SHADOWDIR)`, which represents the *shadow directory* (that is the directory where the `cpps` caches all `.o` and .exe files) of the directory where the source files for the `cpps-make` directive are found.

The advantage of this is that by placing these automatically generated files in the shadow directory, you avoid polluting the directory where the source code is located.

As you can see from the above example, in addition to using this variable in the *embedded Makefile*, it can also be used in the `using` directive to introduce automatically generated source files.

In order to be able to `#include` the generated `.h` file in the source code, you also need to tell the compiler to look for the header files in the *shadow directory*, which is done with the `include-dir` directive.  

With the introduction of the `cpps-make` directive, the *build* stage of the *scan*-*build*-*run* process of `cpps` has been further divided into two sub-stages:

- The first stage *make*s the *embedded Makefile*, generating `.cpp` and `.h` files
- The second stage compiles all `.cpp` files and generates `.exe`  

### `include-dir`
Please refer to the example in `cpps-make`.  

### Variables  
Currently only a variable named `$(SHADOWDIR)` is supported, representing the *shadow directory* of the directory where the source file is located. It can be used in *embedded Makefile* rules, and in `using` directives.  

### Macro directives
From the introduction to `cpps-make` above, we can see that to work with a development environment like *Qt*, which has a complex compilation process, requires a large, rather complex *embedded Makefile* in the form of comments in the C++ source file.

To solve this problem, we provide *macro directives*. A macro directive can be expanded into a long *embedded Makefile*, and users can define their own macros. For *Qt*, we have provided such macro instructions.

The previous example of the ultra-complex *embedded Makefile*, using macro directives, can be simplified to the following form.  

```C++
// cpps-macro qt-moc finddialog.h
// cpps-macro qt-uic propertiesdialog.ui
// cpps-macro qt-rcc resources.qrc
```
where `qt-moc`, `qt-uic`, and `qt-rcc` are the names of macros. The file names that follow are the arguments passed to the macro. These parameters can be referenced in the macro file in the form of `$1`, `$3`, `$3`, ...
The macro files are located in the `.cpps/macro` directory. (Just copy the `marco` folder to `.cpps` directory)
In addition to `$1`, `$3`, `$3`, ... in the macro file, you can also use a built-in function `basename`, which gets foo from `$(basename foo.h)`.  


## Command Line  
What on the command line belongs to `cpps` and what belongs to the *script*?

```
$ cpps [options for cpps] hello.cpp [options and arguments for the hello.cpp]
```
That is, what comes after the script (i.e. hello.cpp) will be passed to the script as command line options and arguments to the script.

Please run the following command to view the command line options of `cpps`.  

```shell
$ cpps --help
```

The command line options overlap to a large extent with the configuration files.  

## Configuration file  
`config.txt`

For GNU/Linux, this file is located under the directory ~\.cpps.

For Windows, it is located under the directory `C:\Users\< Your Name >\.cpps`.

Here is an example (for a more complete example, please refer to `config.txt` in the `examples` directory), you can see that there are 4 sections: `general`, `gcc`, `mingw`, `vc`.  

```INI
[general]
# Specify the default underlying compiler, currently supported: gcc, mingw, vc, clang.
compile-by=vc

[gcc]
# gcc is developer-friendly under linux, so you don't need to specify it.

[clang]

[mingw]
# The following libs4mingw is a directory that I created for myself, which contains some libraries that I compiled using mingw

# you can repeat include-dir and lib-dir many times
# Boost
include-dir = F:\libs4mingw\boost\include
lib-dir = F:\libs4mingw\boost\lib
dll-dir = F:\libs4mingw\boost\lib

# FLTK
include-dir = F:\libs4mingw\fltk-1.3.3
lib-dir = F:\libs4mingw\fltk-1.3.3\lib

# mingw dll
dll-dir = C:\MinGW\bin


[vc]

# vc with vcpkg, don't be too cool
# FLTK
include-dir = F:\vcpkg\installed\x86-windows\include
lib-dir = F:\vcpkg\installed\x86-windows\lib
dll-dir = F:\vcpkg\installed\x86-windows\bin
```

