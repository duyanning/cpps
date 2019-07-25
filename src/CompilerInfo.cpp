#include "config.h"
#include "CompilerInfo.h"
#include "GccCmdLineBuilder.h"
#include "MingwCmdLineBuilder.h"
#include "VcCmdLineBuilder.h"

CC cc = GCC;

CompilerInfo cc_info[] = {
	{
		"gcc",
		".o",
		".gch",
		"g++ -std=c++11 -Wall -c",  // -fmax-errors=2是因为某些错误需要两条错误信息
		"g++ -std=c++11 -Wall",
		"g++ -std=c++11 -fmax-errors=1",
        "-fpch-deps -MMD -MF",
		makeGccCmdLineBuilder
	},
	{
		"mingw",
		".o",
		".gch",
		"mingw32-g++ -std=c++11 -Wall -c",
		"mingw32-g++ -std=c++11 -Wall",
		"mingw32-g++ -std=c++11 -fmax-errors=1",
        "-fpch-deps -MMD -MF",
		makeMingwCmdLineBuilder
	},
	{
        "vc",
		".obj",
		".pch",
		"cl /nologo /EHsc /c",
		"", // vc的头文件不能编译
		"cl /nologo",
        "", // vc的.d文件生成比较复杂，不是一两个编译开关可以搞定的事。我们专门为vc搞了派生类
		makeVcCmdLineBuilder
	},
	{
		"clang",
		".o",
		".gch",
		"clang++ -Wall -c",
		"clang++ -x c++-header -Wall",
		"clang++",
        "-MMD -MF",
		makeGccCmdLineBuilder
	},
};

map<string, CC> map_compiler_name2enum = {
	{ "gcc", CC::GCC },
	{ "mingw", CC::MINGW },
	{ "vc", CC::VC },
	{ "clang", CC::CLANG }
};

//// 如果使用了<thread>或者<future>里的东西，就需要-pthread这个参数。注意，不是-lpthread，不过效果似乎是一样的。
//#if defined(__CYGWIN__) || defined(_WIN32)
//string link_cmd = "g++ -std=c++11 -fmax-errors=1";
//#else
//string gcc_link_cmd = "g++ -std=c++11 -fmax-errors=1 -pthread";
//#endif
