# linux+gcc环境下 
在该目录下运行
./run-all-tests.sh

要将一个目录加入其中，请编辑文件tests。

每个目录下得提供一个./test.sh


可在每个目录下运行
../run-this-test.sh


# windows+vc环境下
test.bat | bash -i -c "FileCheck test.bat"

双引号是必须的

注意：得用cmd，而不能用conemu


用conemu的话，会有类似
←[0m←[
的东西出现在FileCheck的输出中。
这些东西据说是用来控制颜色的。

但在cmd中，就是好的，看不到这些东西，而且有颜色。

