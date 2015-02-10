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
    
不需要什么Makefile

然后执行下边这行即可

cpps hello.cpp

## 如果你在hello.cpp中使用了某个库
比如boost_filesystem

### 在编译环境下

你需要在命令行写-lboost_filesystem才能将该库链接进来

### 在解释环境

你只要在任何一个.cpp文件中写这样一行即可：

    // linklib boost_filesystem
    
## 还支持shebang
在你的hello.cpp文件第一行写上：

    #!/usr/local/bin/cpps

然后执行

    chmod +x hello.cpp
    ./hello.cpp
    
感觉是不是很爽？

只可惜，这一行并不是合法的C++代码

cpps认它，别的编译器却不认它。

