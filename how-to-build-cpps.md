# 如何编译cpps



## linux用gcc的话，提供了CMakeLists.txt
这个东西主要是方便大家在linux上从源码编译安装。

我自己开发时用的是epm，我自己做的另一个C++ project build system engine。



## windows下用vc的话，用visual studio打开cpps.sln然后build solution即可。
因为使用了boost，所以build前得有boost。
用vcpkg装安boost即可
(
vcpkg install boost
或
vcpkg install boost-system boost-filesystem boost-program-options boost-serialization boost-timer boost-chrono
)

然后vcpkg integrate install


这个solution里一共三个程序：
* cpps
* finderror 用来分析vc的编译信息的
* vc-config-gen 用来产生config.txt中与vc相关的配置信息的

## windows下用mingw的话，用codeblocks打开cpps.cbp然后build即可。
因为使用了boost，所以
在build之前，需要在codeblocks中
Settings > Compiler... > Global compiler settings > Custom variables
下，新增一个变量 LIBS4MINGW
其值为(例如)`F:\libs4mingw`  注意，不包括两边的反引号。

在该目录下要有boost目录


