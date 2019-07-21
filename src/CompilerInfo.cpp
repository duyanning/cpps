#include "config.h"
#include "CompilerInfo.h"

CC cc = GCC;

CompilerInfo cc_info[] = {
	{
		"gcc",
		".o",
		".gch",
		"g++ -std=c++11 -Wall -c",  // -fmax-errors=2是因为某些错误需要两条错误信息
		"g++ -std=c++11 -Wall",
		"g++ -std=c++11 -fmax-errors=1"
	},
	{
		"mingw",
		".o",
		".gch",
		"mingw32-g++ -std=c++11 -Wall -c",
		"mingw32-g++ -std=c++11 -Wall",
		"mingw32-g++ -std=c++11 -fmax-errors=1"
	},
	{
        "vc",
		".obj",
		".pch",
		"cl /nologo /EHsc /c",
		//R"("C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.21.27702\bin\Hostx86\x86\cl.exe"  /nologo /EHsc /c)",
		"", // vc的头文件不能编译
		"cl /nologo"
		//R"("C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.21.27702\bin\Hostx86\x86\cl.exe"  /nologo)"
	}
};


//// 如果使用了<thread>或者<future>里的东西，就需要-pthread这个参数。注意，不是-lpthread，不过效果似乎是一样的。
//#if defined(__CYGWIN__) || defined(_WIN32)
//string link_cmd = "g++ -std=c++11 -fmax-errors=1";
//#else
//string gcc_link_cmd = "g++ -std=c++11 -fmax-errors=1 -pthread";
//#endif
