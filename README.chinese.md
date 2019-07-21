# cpps - C++脚本的(伪)解释器 / 不需要任何Makefile的C++项目建构系统引擎 / 支持GCC，MinGW，Visual C++

## 脚本 vs. 程序
我们先来说一下(在我们看来)程序与脚本的区别：

**程序**是独立于你要处理的资料的：程序有自己的目录，可以处理位于不同位置的不同资料。

**脚本**是依附于它所处理的资料的：脚本就扔在资料所在的目录中，脚本里会直接提及资料中的特定文件、特定目录，脱离了这个环境它就运行不了。

一句话，**脚本**的特点就是：_就地编写，就地运行_。

## 为什么需要C++脚本
因为你C++用得最顺手，因为你不会/不熟/不想学其他脚本语言。

早期的脚本语言都比较简单（如dos的批处理文件、unix的shell脚本），很难编写正儿八经的程序。

如今脚本语言越来越强大（如perl、python），都可以用来编写正儿八经的程序了，那本来就很强大的编程语言（如C++），难道就不能放低身段去编写脚本吗？当然可以！

## 为什么说cpps是伪解释器？
因为它并不是传统意义上的解释器，而是背后调用底层编译器(比如gcc)先对源代码进行编译，然后再执行产生的exe程序的。

## cpps相比真解释器的优势
* 支持底层编译器(比如gcc)支持的全部c++语法（必须的）
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

怎么做到的呢？cpps在调用底层编译器编译之前，会先扫描一遍.cpp文件的内容，根据其中的信息，梳理.cpp文件跟.h的依赖关系，并在内存中构建一个依赖关系图。
当你第一次运行脚本时，所有的.cpp都会被编译；但如果你对代码又做了修改，再次运行脚本时，只有变动的.cpp文件才会被重新编译。当然，前提是你的脚本是由多个.cpp文件组成的(.cpp文件越小，优势越明显)。

任何build system都包括两个部分：
- 一个build system engine
- 一些由用户编写的文件

例如：make和Makefile，cmake和CMakeLists.txt。

一般来说，engine做的事情越多，需要用户编写的部分就越少。

所以，cpps与其说是一个解释器，不如说是一个高级的build system engine，它只需要用户在.cpp中以解释器指令的形式提供少许信息就可以构建一个完整的build system。

## 获取
```
git clone https://github.com/duyanning/cpps.git
```

## 安装前准备工作
以ubuntu为例
```ShellSession
sudo apt-get install libboost-filesystem-dev
sudo apt-get install libboost-program-options-dev
sudo apt-get install libboost-serialization-dev
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

## 如果脚本由一个.cpp文件组成
比如hello.cpp

那你执行下面这行即可：
```ShellSession
cpps hello.cpp
```

## 如果脚本由多个.cpp文件组成
比如你的c++代码位于目录`~/work/hello`下，共有三个文件：

hello.cpp

foo.cpp

foo.h

如果你在hello.cpp中调用了foo.cpp中的某个函数，那么

### 在gcc的编译环境下

你需要在hello.cpp中写
```c++
#include "foo.h"
```
然后将foo.cpp也加进你的Makefile中。

### 在cpps的解释环境下

你只需要在上面这行`#include`后添加一条注释，如下：
```c++
#include "foo.h" // usingcpp
```    
然后执行下边这行即可
```ShellSession
cpps hello.cpp
```    

如果你的.cpp文件与.h文件不同名的话，那就要这样写：
```c++
#include "foo.h" // using fooandbar.cpp
```

## 如果你在hello.cpp中使用了某个库
比如pthread

### 在gcc的编译环境下

你需要在gcc的命令行写`-lpthread`才能将该库链接进来

### 在cpps的解释环境下

你只要在任何一个.cpp文件中加上这样的注释即可：
```c++
// linklib pthread
```    
## 预编译头文件
你还可以通过预编译头文件来加速cpps的编译过程

比如你打算将std.h搞成预编译头文件

那么只要在包含std.h的某个.cpp文件中加上这样的注释即可
```c++
#include "std.h" // precompile
```
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

只可惜，这一行并不是合法的C++代码

cpps认它，别的编译器却不认它。

更好的做法是：不要在.cpp文件中直接添加shebang，而是新建一个扩展名为.cpps的文件，比如：
    
    hello.cpps
    
然后在这个文件中的第一行写shebang，第二行给出.cpp脚本的路径（绝对路径、相对路径都可以），如下：

    #!/usr/bin/env cpps
    hello.cpp

然后将这个.cpps文件设置成可执行的
```ShellSession
chmod +x hello.cpps
./hello.cpps
```
咦？
好像在个shell脚本中调用cpps hello.cpp即可达到相同效果？！

也不完全是，用shell脚本的话会多执行一次shell，而且也无法通过扩展名提醒你这是一个C++脚本。

## 干干净净运行
```ShellSession
cpps --clear hello.cpp
```    
相当于先删除之前生成的.o文件、exe文件，然后再从头build，然后再运行。

如果你用新版本的cpps运行以前运行过的C++程序时遇到问题，可以这样试一下。


## 我想得到cpps在背后生成的那个可执行文件
```ShellSession
cpps --build hello.cpp -o hello
```    
cpps就会把它生成的那个可执行文件给你拷贝一份

## 使用MingGW作为底层编译器
需要装了MingW，并确保`C:\MinGW\bin`在你的PATH环境变量中。

如果你想使用boost库，可以从[此处](https://sourceforge.net/projects/boostmingw/)下载别人编译好的(因为boost并没有提供对mingw的官方支持)。

比如我们将下载的boost放在`D:\libs4mingw`下


然后，在用cpps执行程序时，你还需要在命令行指定boost头文件(.h)所在目录、库文件(.a)所在目录，以及动态库文件(.dll)所在目录，如下：
```ShellSession
cpps -c mingw hello.cpp -ID:\libs4mingw\boost\include -LD:\libs4mingw\boost\lib --dll-dir=D:\libs4mingw\boost\lib
```
每次都要指定太麻烦？

你可以在`c:\Users\<Your Name>\.cpps`目录下创建一个名为`config.txt`的文件，在里面写上：
```INI
[mingw]
include-dir = D:\libs4mingw\boost\include
lib-dir = D:\libs4mingw\boost\lib
dll-dir = D:\libs4mingw\boost\lib
```
这样你以后只需要运行
```ShellSession
cpps hello.cpp
```
即可。

config.txt中的`include-dir`、`lib-dir`和`dll-dir`可以在多行中出现。

如果你在资源管理器中不好建立名字里带点的目录，你只要用cpps执行一次.cpp文件即可建立该目录。

## 使用Visual C++作为底层编译器
确保minised.exe与finderror.exe跟cpps.exe放在一起(这俩随cpps提供)。
```ShellSession
cpps -c vc main.cpp
```
像上面这样，使用-c参数指定使用vc。

因为vc提供的选项没有gcc那么丰富，cpps对vc的控制无法通过现成的选项来进行，还需要分析vc的输出。
在分析过程中，cpps会调用gnuwin32提供的minised.exe(就在minised文件夹下，你将它跟cpps.exe放一起)。

如果你不想每次运行cpps之前，都要先在cmd窗口中运行Visual C++提供的vcvars32.bat来设置环境变量，你可以这样做：

* 新打开一个干净的cmd窗口
* 在这个窗口中运行vcvars32.bat来设置环境变量
* 在这个窗口中运行cpps提供的小程序vc-config-gen.exe
* 然后将其输出复制粘贴到配置文件config.txt的vc区块即可。


## 解释器指令汇总
### using
例子：
```c++
#include "foo.h" // using foo.cpp
```
### usingcpp
例子：
```c++
#include "foo.h" // usingcpp
```
等价于
```c++
#include "foo.h" // using foo.cpp
```
### `linklib`和`<compiler>-linklib`
例子：
```c++
#include <FL/Fl.H> // linklib fltk
```
如果同一个库，在不同编译器下有不同的名字，你可以用`<compiler>-linklib`。
### `<compiler>-extra-compile-flags`和`<compiler>-extra-link-flags`
例子：如果你在Windows下用MinGW编译使用FLTK的程序，除了
```c++
// linklib fltk
```
你还需要在.cpp文件中加上以下指令
```c++
// mingw-extra-compile-flags: -DWIN32
// mingw-extra-link-flags: -mwindows -lole32 -luuid -lcomctl32
```
## 命令行选项
请运行以下命令来查看：
```ShellSession
cpps --help
```
## 命令行上的东西，哪些属于cpps？哪些属于脚本？
```ShellSession
cpps [cpps的命令行选项...] 脚本 [脚本的命令行选项与命令行参数...]
```
也就是说，脚本(即xxx.cpp)之后的东西将传给脚本，作为脚本的命令行选项与参数

## 配置文件
`config.txt`

对于GNU/Linux来说，它位于目录`~\.cpps`下。

对于Windows来说，它位于目录`c:\Users\<Your Name>\.cpps`下。

下面是个例子，你可以看到其中有4个section：general、gcc、mingw、vc。

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