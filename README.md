# cpps - C++代码的(伪)解释器

## 如果脚本由一个.cpp文件组成
比如hello.cpp

那你执行下面这行即可：

cpps hello.cpp

## 如果脚本由多个.cpp文件组成
以一个例子来说明

如果你的c++代码位于目录~/work/hello下：

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


## 如果你在foo.cpp中使用了某个库
比如boost_filesystem

### 在编译环境下

你需要在命令行写-lboost_filesystem才能将该库链接进来

### 在解释环境

你只要在任何一个.cpp文件中写这样一行即可：

    // linklib boost_filesystem
