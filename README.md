# cpps - C++脚本的(伪)解释器

## 脚本 vs. 程序
程序是独立于你要处理的资料的：程序有自己的目录，可以处理位于不同位置的不同资料。

脚本是依附于它所处理的资料的：脚本就扔在资料所在的目录中，脚本里会直接提及资料中的特定文件、特定目录，脱离了这个环境它就运行不了。

一句话，脚本的特点就是：就地编写，就地运行。

## 为什么需要C++脚本
因为你C++用得最顺手，因为你不会/不熟/不想学其他脚本语言。

早期的脚本语言都比较简单（如dos的批处理文件、unix的shell脚本），很难编写正儿八经的程序。

如今脚本语言越来越强大（如perl、python），都可以用来编写正儿八经的程序了，那本来就很强大的编程语言（如C++），难道就不能放低身段去编写脚本吗？当然可以！

## 为什么说是伪解释器？
因为它并不是传统意义上的解释器，而是背后调用gcc先对源代码进行编译，然后再执行产生的exe程序的。

## 相比真解释器的优势
* 支持gcc支持的全部c++语法（必须的）
* 脚本执行速度快（是gcc产生的机器语言代码在执行）

## 相比真解释器的劣势
* 没有REPL，不能交互式地执行
* 不能独立地嵌入其他程序（除非你不介意自己的程序也依赖于gcc）

## 其他你可能关心的方面
* 不会弄脏你的工作目录（不会在你的工作目录中留下.o文件、可执行文件）
* 支持多个.cpp文件且不需要工程文件（Makefile、CMakeLists.txt之类）
* 智能地决定哪些.cpp需要重新编译
* 自动生成、使用、管理预编译头文件
* 可以链接库
* 支持shebang

## 这一切是怎么做到的？
因为C++代码编译的时间也不短，所以cpps会缓存前一次运行所产生的.o文件、exe文件。

cpps内建了一个全透明的build system，当你第一次运行脚本之后，如果你对代码又做了修改，再次运行脚本时，只有变动的.cpp文件才会被重新编译。当然，前提是你的脚本是由多个.cpp文件组成的。

所以说，cpps与其说是一个解释器，不如说是一个高级make+Makefile自动生成工具。

## 编译安装

    mkdir build-cpps
    cd build-cpps
    cmake ../cpps
    make
    make test

如果没有问题的话

    sudo make install
    
特别注意：

我自己编译时用的是

* gcc 4.9.1
* boost 1.57.0 （只用到了filesystem、program_options两个子库）

## 如果脚本由一个.cpp文件组成
比如hello.cpp

那你执行下面这行即可：

    cpps hello.cpp

## 如果脚本由多个.cpp文件组成
比如你的c++代码位于目录~/work/hello下，共有三个文件：

hello.cpp

foo.cpp

foo.h

如果你在hello.cpp中调用了foo.cpp中的某个函数，那么

### 在gcc的编译环境下

你需要在hello.cpp中写

    #include "foo.h"
    
然后将foo.cpp也加进你的Makefile中。

### 在cpps的解释环境下

你只需要在上面这行#include后添加一条注释，如下：

    #include "foo.h" // usingcpp
    
然后执行下边这行即可

    cpps hello.cpp

如果你的.cpp文件与.h文件不同名的话，那就要这样写：

    #include "foo.h" // using fooandbar.cpp


## 如果你在hello.cpp中使用了某个库
比如pthread

### 在gcc的编译环境下

你需要在gcc的命令行写-lpthread才能将该库链接进来

### 在cpps的解释环境下

你只要在任何一个.cpp文件中加上这样的注释即可：

    // linklib pthread
    
## 预编译头文件
你还可以通过预编译头文件来加速cpps的编译过程

比如你打算将std.h搞成预编译头文件

那么只要在包含std.h的某个.cpp文件中加上这样的注释即可

    #include "std.h" // precompile
    
## 还支持shebang
在你的hello.cpp文件第一行写上：

    #!/usr/bin/env cpps

然后执行

    chmod +x hello.cpp
    ./hello.cpp
    
感觉是不是很爽？

只可惜，这一行并不是合法的C++代码

cpps认它，别的编译器却不认它。

更好的做法是：不要在.cpp文件中直接添加shebang，而是新建一个扩展名为.cpps的文件，比如：
    
    hello.cpps
    
然后在这个文件中的第一行写shebang，第二行给出.cpp脚本的路径（绝对路径、相对路径都可以），如下：

    #!/usr/bin/env cpps
    hello.cpp

然后将这个.cpps文件设置成可执行的

    chmod +x hello.cpps
    ./hello.cpps

咦？
好像在个shell脚本中调用cpps hello.cpp即可达到相同效果？！

## 干干净净运行

    cpps --clear hello.cpp
    
相当于先删除之前生成的.o文件、exe文件，然后再从头build，然后再运行。

如果你用新版本的cpps运行以前运行过的C++程序时遇到问题，可以这样试一下。


## 我想得到cpps在背后生成的那个可执行文件

    cpps --build hello.cpp -o hello
    
cpps就会把它生成的那个可执行文件给你拷贝一份

## Windows用户
需要装了MingW，并确保C:\MinGW\bin在你的PATH环境变量中。

如果你想使用boost库，可以从此处

    https://sourceforge.net/projects/boostmingw/

下载别人编译好的(因为boost并没有提供对mingw的官方支持)。

比如我们将下载的boost放在D:\libs4mingw下

为了让cpps正常运行，你需要在PATH环境变量中添加

    D:\libs4mingw\boost\lib

这是为了让你的程序在运行时能够找到需要的动态链接库。

然后，你还需要在命令行指定boost头文件(.h)所在目录和库文件(.a)所在目录，如下：

    cpps hello.cpp -I D:\libs4mingw\boost\include -L D:\libs4mingw\boost\lib

每次都要指定太麻烦？

你可以在C:\.cpps目录下创建一个名为config.txt的文件，在里面写上：

    include-dir = D:\libs4mingw\boost\include
    lib-dir = D:\libs4mingw\boost\lib

这样你以后只需要运行

    cpps hello.cpp

即可。


config.txt中的include-dir和lib-dir可以在多行中出现。

如果你在资源管理器中不好建立名字里带点的目录，你只要用cpps执行一次.cpp文件即可建立该目录。

