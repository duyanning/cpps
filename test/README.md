回归测试

# linux+gcc环境下 
要运行所有测试用例，在本目录下执行
    ./run-all-tests.sh

要将一个目录加入其中，请编辑文件tests。

要运行单个测试用例，可在用例目录下执行
    ../run-this-test.sh

每个目录下得提供一个./test.sh

# windows+vc环境下

要运行所有测试用例，在本目录下执行
    run-all-tests.bat

要运行单个测试用例，可在用例目录下执行
    ..\run-this-test.bat

每个目录下得提供一个test.bat


注意：得用cmd或powershell，而不能用conemu
用conemu的话，会有类似
←[0m←[
的东西出现在FileCheck的输出中。
这些东西据说是用来控制颜色的。
但在cmd中，就是好的，看不到这些东西，而且有颜色。


# windows+mingw环境下
要运行所有测试用例，在本目录下执行
    run-all-tests-mingw.bat

要运行单个测试用例，可在用例目录下执行
    ..\run-this-test-mingw.bat

每个目录下得提供一个test-mingw.bat

# 将一个测试用例在gcc/mingw/vc三者间调整
../sh2bat.sh  run-gcc.sh vc > run-vc.sh
../sh2bat.sh  run-gcc.sh mingw > run-mingw.sh
