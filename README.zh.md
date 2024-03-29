[![English](https://img.shields.io/badge/language-English-blue.svg)](https://github.com/duyanning/cpps/blob/master/README.md)

# 简介

从表象上来看，cpps是C++脚本的解释器，它可以像python直接执行.py文件一样，直接执行.cpp文件。

就本质上而言，cpps其实是一个不需要任何Makefile的C++项目建构系统引擎。它调用底层编译器(GCC，MinGW，Visual C++, Clang)将.cpp文件编译成.exe文件，然后执行生成的.exe文件，但并不会在目录中留下.o文件或是.exe文件。

# 基本用法

## 如果脚本只包含一个.cpp文件
比如你有一个hello.cpp
```C++
#include <iostream>

using namespace std;

int main()
{
    cout << "hello, world" << endl;
}
```

那你执行下面这行即可：
```shell
$ cpps hello.cpp
```

## 如果脚本由多个.cpp文件组成
比如你的c++代码位于目录`~/work/hello`下，共有三个文件：main.cpp、foo.cpp、foo.h，内容分别如下：
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

如果你使用make+Makefile的方案，除了源文件外，你还需要准备一个Makefile，内容如下：

```Makefile
hello: main.o foo.o
    g++ -o hello main.o foo.o

main.o: main.cpp foo.h
    g++ -c main.cpp

foo.o: foo.cpp foo.h
    g++ -c foo.cpp
```

然后执行：
```shell
$ make
$ ./hello
```

但如果你使用cpps的话，只需在main.cpp中找到下面这行
```C++
#include "foo.h"
```
并在这行最后添加一条注释，使其变成：
```C++
#include "foo.h" // usingcpp
```

然后执行
```shell
$ cpps main.cpp
```
即可。


除了usingcpp指令外，你也可以使用using指令，以上那种写法等同于：
```c++
#include "foo.h" // using foo.cpp
```
这种写法主要用在.cpp文件与.h文件不同名的情况下。


## 链接某个库
如果你在hello.cpp中，使用了某个库，比如pthread，

那么，在gcc的编译环境下

你需要在gcc的命令行写`-lpthread`才能将该库链接进来。

在cpps的解释环境下，

你只要在任意一个.cpp文件中加上这样的注释即可：
```C++
// linklib pthread
```
推荐做法是，如果哪个.cpp文件用了这个库，就把linklib指令写在哪个.cpp文件中。

如果多个.cpp文件都用了同一个库的话，写多次也没问题。

觉得烦，不如集中写在main函数所在的.cpp中。


# 理念

## 脚本 vs. 程序
我们先来说一下(在我看来)程序与脚本的区别：

**程序**是独立于你要处理的资料的：程序有自己的目录，可以处理位于不同位置的不同资料。

**脚本**是依附于它所处理的资料的：脚本就扔在资料所在的目录中，
脚本里会直接提及资料中的特定文件、特定目录，脱离了这个环境它就运行不了。

除此之外，脚本还有一个特点，就是它只有两个环节：编写、运行。

而程序通常有三个环节：编写、build、运行。
而且build环节生成的可执行文件，并不位于将要处理的资料所在的位置。很难连同资料一起发布。
build过程还会产生一大堆中间文件，弄脏资料所在目录。

一句话，**脚本**的特点就是：**就地编写，就地运行**。

这正是cpps追求的目标。

## 为什么需要C++脚本
因为你C++用得最顺手，因为你不会/不熟/不想学其他脚本语言。

早期的脚本语言都比较简单（如dos的批处理文件、unix的shell脚本），很难编写正儿八经的程序。

如今脚本语言越来越强大（如perl、python），都可以用来编写正儿八经的程序了，那本来就很强大的编程语言（如C++），难道就不能放低身段去编写脚本吗？当然可以！

## 为什么说cpps是伪解释器？
因为它并不是传统意义上的解释器，而是背后调用底层编译器(比如gcc)先对源代码进行编译，然后再执行产生的exe程序的。

## cpps相比真解释器的优势
* 支持底层编译器(比如gcc)支持的全部c++语法（C++源码由底层编译器编译，所有的cpps指令皆以C++注释的形式存在）
* 脚本执行速度快（是底层编译器产生的机器语言代码在执行）

## cpps相比真解释器的劣势
* 没有REPL，不能交互式地执行
* 不能独立地嵌入其他程序（除非你不介意自己的程序也依赖于底层编译器）

## 其他你可能关心的方面
* 不会弄脏你的工作目录（不会在你的工作目录中留下.o文件、可执行文件）
* 支持多个.cpp文件的项目且不需要任何类型的工程文件（Makefile、CMakeLists.txt之流一个也不要，只有.cpp跟.h）
* 智能地决定哪些.cpp需要重新编译
* 自动生成、使用、管理预编译头文件
* 可以链接库
* 支持shebang

## 这一切是怎么做到的？
因为C++代码编译的时间也不短，每次“解释”之前都先编译一遍肯定不能接受，所以cpps会缓存前一次运行所产生的.o文件、exe文件。(缓存位于`~/.cpps/cache`中)

用cpps执行C++脚本，会经历以下几个阶段：扫描 > build > 执行

其中build是我们所熟悉的编译链接过程。


扫描是指cpps在调用底层编译器编译之前，会先扫描一遍.cpp文件的内容，提取其中的cpps指令。
这些指令有些用来指示cpps将哪些.cpp文件纳入工程，有的用来标记需要预编译的头文件，有些用来告诉cpps需要链接哪些库，使用哪些编译或链接开关。
然后，cpps会在内存中构建一张反映exe文件、obj文件、.cpp文件、.h文件之间依赖关系的依赖关系图。
当你第一次运行脚本时，所有的.cpp都会被编译；但如果你对代码又做了修改，再次运行脚本时，只有变动的.cpp文件才会被重新编译。
当然，前提是你的脚本是由多个.cpp文件组成的(.cpp文件越小，优势越明显)。(运行cpps时，你可以试下加上-v参数，看看cpps有没有编译不用编译的文件)
不仅build的过程是这样，扫描的过程也是如此，只有变动过的.cpp才会被重新扫描。


## cpps的哲学

任何build system都包括两个部分：

- 一个build system engine(如make)
- 一些由用户编写的文件(如Makefile)


一般来说，engine做的事情越多，需要用户编写的部分就越少。

所以，cpps与其说是一个解释器，不如说是一个高级的build system engine，它只需要用户在.cpp中以解释器指令的形式提供少许信息就可以构建一个完整的build system。
可以说对于cpps这个build system engine来说，Makefile的信息是记录在.cpp文件中的。

你执行python脚本前需要写Makefile，需要手动make吗？不需要！

所以执行C++脚本前也不应该手写Makefile跟手动make。

cpps的`usingcpp`指令用来跟`#include`配合，二者合起来的作用类似python等脚本语言的import。

通过这种方式弥补C++中.h(接口)跟.obj(实现)没有二合一的缺憾。C++背着C时代旧式工具链的包袱。
人家Java的.class文件就相当于.h跟.obj二合一，还有python的.pyc也相当于.h跟.obj二合一。
.java/.py不过是.class/.pyc的高级语言形式，相当于.h跟.cpp二合一。

# 安装
## 获取
### GNU/Linux
```
git clone https://github.com/duyanning/cpps.git
```
### Windows
提供了已经编译好的二进制文件。

下载，解压后得到一个文件夹，其中包含了cpps.exe与其他几个.exe。

然后该目录加入环境变量PATH即可。

(提供的二进制文件，从压缩包的名字可以看出，有用vc编译，有用mingw编译的。这个只是说cpps的二进制文件是用vc或mingw编译的，并不代表cpps执行时使用哪个底层编译器，执行cpps时用哪个底层编译器要在配置文件或命令行中指定)

## 安装前准备工作
以ubuntu为例
```ShellSession
sudo apt-get install libboost-filesystem-dev libboost-program-options-dev libboost-serialization-dev libboost-system-dev libboost-regex-dev libboost-timer-dev libboost-chrono-dev
```

## 编译安装
```ShellSession
mkdir build-cpps
cd build-cpps
cmake ../cpps
make
make test
```

如果没有问题的话
```ShellSession
sudo make install
```

我自己编译时用的是(注意版本号)：

* gcc 4.9.1
* boost 1.57.0

用较低版本可能会遇到编译或链接问题。

# 使用

最基本的用法已在“简介”部分介绍过了，此处不再赘述，只说一些高级用法。



## 预编译头文件
你还可以通过预编译头文件来加速cpps的编译过程

比如你打算将std.h搞成预编译头文件

那么只要在包含std.h的某个.cpp文件中加上这样的注释即可
```c++
#include "std.h" // precompile
```

预编译头文件对于缩短C++程序的编译时间，效果非常显著。强烈建议使用！
而且同一个目录下的C++脚本可以共享同一个预编译头文件。

我一般都是搞一个std.h，里面把所有的标准库、boost库的头文件全都包含上。

用cpps你可以执行`cpps -g hello.cpp`，除了`hello.cpp`外，还将生成`std.h`和`std.cpp`两个文件。
## 还支持shebang
在你的hello.cpp文件第一行写上：
```c++
#!/usr/bin/env cpps
```
然后执行
```ShellSession
chmod +x hello.cpp
./hello.cpp
```
感觉是不是很爽？

只可惜，这一行并不是合法的C++代码。

它会导致你用编译器编译hello.cpp时报错(虽然cpps通过一些魔法使得自己的底层编译器在编译时不会报错)。

更好的做法是：不要在.cpp文件中直接添加shebang，而是新建一个扩展名为.cpps的文件，比如`hello.cpps`。
    
然后在这个文件中的第一行写shebang，第二行给出.cpp脚本的路径（绝对路径、相对路径都可以），如下：

    #!/usr/bin/env cpps
    hello.cpp

然后将这个.cpps文件设置成可执行的
```shell
$ chmod +x hello.cpps
$ ./hello.cpps
```
咦？
好像在个shell脚本中调用`cpps hello.cpp`即可达到相同效果？！

也不完全是，用shell脚本的话会多执行一次shell，而且也无法通过扩展名提醒你这是一个C++脚本。

## 干干净净运行
```shell
$ cpps --clear hello.cpp
```
相当于先删除之前生成的.o文件、exe文件，然后再从头build，然后再运行。

如果你用新版本的cpps运行以前运行过的C++程序时遇到问题，可以这样试一下。

如果还有问题，直接把缓存目录`.cpps/cache`删除掉即可。


## 我想得到cpps在背后生成的那个可执行文件
```shell
$ cpps --build hello.cpp -o hello
```
cpps就会把它生成的那个可执行文件给你拷贝一份。

## 使用MingGW作为底层编译器
需要装了MingW，并确保`C:\MinGW\bin`在你的PATH环境变量中。

如果你想使用boost库，可以从[此处](https://sourceforge.net/projects/boostmingw/)下载别人编译好的(因为boost并没有提供对mingw的官方支持)。

比如我们将下载的boost放在`D:\libs4mingw`下


然后，在用cpps执行程序时，你还需要在命令行指定boost头文件(.h)所在目录、库文件(.a)所在目录，以及动态库文件(.dll)所在目录，如下：
```shell
$ cpps -c mingw hello.cpp -ID:\libs4mingw\boost\include -LD:\libs4mingw\boost\lib --dll-dir=D:\libs4mingw\boost\lib
```
每次都要指定太麻烦？

你可以在`c:\Users\< Your Name >\.cpps`目录下创建一个名为`config.txt`的文件，在里面写上：
```INI
[mingw]
include-dir = D:\libs4mingw\boost\include
lib-dir = D:\libs4mingw\boost\lib
dll-dir = D:\libs4mingw\boost\lib
```
这样你以后只需要运行
```shell
$ cpps hello.cpp
```
即可。

config.txt中的`include-dir`、`lib-dir`和`dll-dir`可以在多行中出现。

如果你在资源管理器中不好建立名字里带点的目录，你只要用cpps执行一次.cpp文件即可建立该目录。

## 使用Visual C++作为底层编译器
确保minised.exe与finderror.exe跟cpps.exe放在一起(这俩随cpps提供)。
```shell
> cpps -c vc main.cpp
```
像上面这样，使用`-c`参数指定使用vc。

因为vc提供的选项没有gcc那么丰富，cpps对vc的控制无法通过现成的选项来进行，还需要分析vc的输出。
在分析过程中，cpps会调用[gnuwin32](http://gnuwin32.sourceforge.net/)项目提供的minised.exe(就在minised文件夹下，你将它跟cpps.exe放一起)。

熟悉Visual C++命令行编译工具的朋友都知道，在执行cl之前，得先运行Visual C++提供的`vcvars32.bat`以设置必要的环境变量。
如果你不想每次运行cpps之前，都要先在cmd窗口中运行vcvars32.bat，你可以这样做：

- 新打开一个干净的cmd窗口
- 在这个窗口中运行vcvars32.bat来设置环境变量
- 在这个窗口中运行cpps提供的小程序vc-config-gen.exe
- 然后将其输出复制粘贴到配置文件config.txt的vc区块即可。

这样做了之后，就可以直接运行cpps，而无须先执行`vcvars32.bat`了。
注意：Visual Studio每次升级后，vc所在的路径可能会发生变化。此时就需要重新运行vc-config-gen.exe，并用其给出的输出去更新config.txt中的过时部分。

## 解释器指令汇总

绝大多数cpps指令，以C++单行注释的形式存在。极少数也可以C++多行注释的形式存在。

所有的指令都是写在.cpp文件中的，写在.h中没用，因为cpps根本不会扫描.h文件中的内容。

### `usingcpp`与`using`
简介中已经说过，不再重复。

### `linklib`与`<compiler>-linklib`
比如一个使用了FLTK库的程序，可以以下形式链接fltk。
```C++
#include <FL/Fl.H> // linklib fltk
```
如果同一个库，在不同编译器下有不同的名字，你可以用`<compiler>-linklib`这种形式代替`linklib`。
其中`<compiler>`可以是gcc、vc、mingw、clang等。

库后边的扩展名，你加也可以，不加也可以。还支持通配符(支持的通配符与shell有关)。

### `<compiler>-extra-compile-flags`与`<compiler>-extra-link-flags`
有些程序需要特殊的编译/链接选项，比如你在Windows下用MinGW编译使用FLTK的程序，除了
```C++
// linklib fltk
```

你还需要在.cpp文件中加上以下指令
```C++
// mingw-extra-compile-flags: -DWIN32
// mingw-extra-link-flags: -mwindows -lole32 -luuid -lcomctl32
```

### `<compiler>-extra-compile-flags(local)`
这条指令最后带个`(local)`，与`<compiler>-extra-compile-flags`的不同在于：
前者仅对当前源文件起作用，后者对所有源文件起作用。

### `cpps-make`
现实中的C++工程，源代码除了C++的.cpp跟.h外，还会涉及到其他一些类型的文件。
这些文件经过编译之后，会产生.cpp或.h文件，而这些生成的.cpp或.h文件，需要被编译进来。

比如FLTK的界面描述文件.fl，经由fluid编译之后会产生.cpp文件和.h文件。

又比如Qt的界面描述文件.ui，经由uic编译之后会产生.h文件；

包含Q_OBJECT的.h文件经由moc编译后会产生.cpp文件；

资源文件.qrc经rcc编译后会产生.cpp文件。

对于这类千变万化的东西，cpps提供了嵌入式的Makefile。

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

`using`指令后的`nocheck`告诉cpps，这个叫`finddialog.h.cpp`的文件是build阶段生成出来的，即便扫描阶段不存在，也不要报错。

`cpps-make`指令引入一条用户自定义规则，语法同Makefile，当然，不会有普通Makefile那么完备。`moc`那条规则采用的是多行cpps-make语法，跟我们熟悉的语法很像。

`uic`与`rcc`两条规则采用的是单行`cpps-make`语法。

`rcc`那条规则中还使用了变量`$(SHADOWDIR)`，该变量代表`cpps-make`指令所处源文件所在目录的影子目录(就是cpps缓存.o文件的目录)。这样做的好处是，将这些自动生成的文件放在影子目录中，可以避免污染源码所在目录。从上面的例子中可以看到，除了在内嵌Makefile中使用该变量外，还可在`using`指令中使用该变量，以引入自动生成的源文件。

为了能够在源码中`#include`生成的`.h`文件，还需要告诉编译器在影子目录中寻找头文件，这是通过`include-dir`指令实现的。

引入cpps-make指令之后，cpps的“scan-build-run”三阶段中的build阶段就被进一步划分成了两个子阶段：

- 第一个阶段执行内嵌的Makefile，产生.cpp和.h
- 第二个阶段编译所有.cpp，产生.exe

### `include-dir`
请参考`cpps-make`中的例子。

### 变量
目前仅支持一个名为`$(SHADOWDIR)`的变量，代表源文件所在目录的影子目录。
可在内嵌的Makefile规则，以及using指令中使用。

### 宏指令
从上面对`cpps-make`的介绍，我们可以看到，要跟Qt这类具有复杂编译流程的开发环境配合起来，需要在C++源文件中以注释的形式，写一大段相当复杂的嵌入式Makefile。

为了解决这个问题，我们提供了宏指令。一条宏指令可以展开为很长一段嵌入式Makefile。用户可以自己定义宏指令。针对Qt，我们已经提供了这样的宏指令。

之前那个超复杂的嵌入式Makefile的例子，使用宏指令，就可以简化为以下形式：

```C++
// cpps-macro qt-moc finddialog.h
// cpps-macro qt-uic propertiesdialog.ui
// cpps-macro qt-rcc resources.qrc
```
其中`qt-moc`、`qt-uic`、`qt-rcc`是宏的名字。后边的文件名是传递给宏的参数。在宏文件中可以以`$1`、`$3`、`$3`、……的形式来引用这些参数。

宏文件位于`.cpps/macro`目录下。(将marco文件夹复制到.cpps下即可)

在宏文件中除了`$1`、`$3`、`$3`、……，还可以使用一个内置函数`basename`，它可以从
`$(basename foo.h)`
得到`foo`。


## 命令行
命令行上的东西，哪些属于cpps？哪些属于脚本？
```shell
$ cpps [cpps的命令行选项...] hello.cpp [脚本的命令行选项与命令行参数...]
```
也就是说，脚本(即hello.cpp)之后的东西将传给脚本，作为脚本的命令行选项与参数

请运行以下命令来查看命令行选项：
```shell
$ cpps --help
```

命令行选项跟配置文件在很大程度上是重叠的。

## 配置文件
`config.txt`

对于GNU/Linux来说，它位于目录`~/.cpps`下。

对于Windows来说，它位于目录`C:\Users\< Your Name >\.cpps`下。

下面是个例子(更完备的例子请参考examples目录下的config.txt)，你可以看到其中有4个section：general、gcc、mingw、vc。

```INI
[general]
# 指定默认的底层编译器，目前支持：gcc、mingw、vc三种。
compile-by=vc

[gcc]
# gcc因为在linux系统下，对开发者比较友好，一般不用专门配

[mingw]
# 下面的libs4mingw是我自己创建的一个目录，里面是我用mingw编译生成的一些库

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

# vc配合vcpkg，不要太爽
# FLTK
include-dir = F:\vcpkg\installed\x86-windows\include
lib-dir = F:\vcpkg\installed\x86-windows\lib
dll-dir = F:\vcpkg\installed\x86-windows\bin
```

