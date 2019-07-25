@REM 这个文件换行用dos风格才行，用unix风格换行会报错
@REM cl /EHsc /showIncludes main.cpp > main.inc

@REM 剔除库文件；剩下的写入one.txt后并从标准输出中删除。有bug：foo.h在one.txt中出现两次
TYPE main.inc | ..\minised -e "/Note: including file:[ \t]\+[A-Z]:\\Program Files/d" -e "/Note: including file:/w bug.txt" -e "/Note: including file:/d"

@REM 把最后一个-e的内容用管道再次给minised，就可以避免这个bug
TYPE main.inc | ..\minised -e "/Note: including file:[ \t]\+[A-Z]:\\Program Files/d" -e "/Note: including file:/w ok.txt" | ..\minised -e "/Note: including file:/d"

@REM 可以运行a.sh以跟正常sed的行为做对比


