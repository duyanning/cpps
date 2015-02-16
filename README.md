# cpps - C++脚本的(伪)解释器

## 为什么说是(伪)解释器？

因为它并不是传统意义上的解释器，而是背后调用gcc先对源代码进行编译，然后再执行产生的exe程序的。

C++代码本身就不适合解释执行，

而我想要的不过是脚本的好处（而非解释器的）：

* 就地编写，就地运行
* 不用创建复杂的工程
* 从edit->build->run的循环中删掉build的环节

因为C++代码编译的时间也不短，所以cpps会缓存第一次运行所产生的exe文件（放心，不会弄脏你的目录）。

另外，cpps还内建了一个全透明的build system，当你第一次运行脚本之后，如果你对代码又做了修改，再次运行脚本时，只有变动的部分才会被重新编译。

当然，前提是你的脚本由多个.cpp文件组成。


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

### 在编译环境下

你需要在hello.cpp中写

    #include "foo.h"
    
然后将foo.cpp也加进你的Makefile中。

### 在解释环境下

你只需要给上面这行#include添加一条注释即可

    #include "foo.h" // using foo.cpp
    
这就相当于python的import

不需要什么Makefile

然后执行下边这行即可

cpps hello.cpp


如果你的.h文件与.cpp文件同名的话，那就更简单了

    #include "VeryVeryVeryLong.h" // usingcpp
   
上面这行就相当于

    #include "VeryVeryVeryLong.h" // using VeryVeryVeryLong.cpp
    
不但少打很多字，而且避免了文件改名后需要修改多处的问题

## 如果你在hello.cpp中使用了某个库
比如pthread

### 在编译环境下

你需要在命令行写-lpthread才能将该库链接进来

### 在解释环境

你只要在任何一个.cpp文件中加上这样的注释即可：

    // linklib pthread
    
## 预编译头文件
你还可以通过预编译头文件来加速

比如你打算将std.h搞成预编译头文件

那么只要在某个.cpp文件中加上这样的注释

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
好像在个shell脚本中调用cpps hello.cpp即可

## 干干净净运行

    cpps --clear hello.cpp
    
相当于先clear all，再build，然后运行。

如果你用新版本的cpps运行以前运行过的C++程序时遇到问题，可以这样试一下。


## 我想得到cpps在背后生成的那个可执行文件

    cpps --build hello.cpp -o hello
    
cpps就会把它生成的那个可执行文件给你拷贝一份


