#ifndef COMPILERINFO_H
#define COMPILERINFO_H

#include "CmdLineBuilder.h"

// 目前支持的底层编译器
enum CC {
	GCC = 0,
	MINGW,
	VC
};

extern CC cc;

struct CompilerInfo {
    const string compiler_name;
	const string obj_ext;
	const string pch_ext;
	const string compile_cpp_cmd;
	const string compile_h_cmd;
	const string link_cmd;
	CmdLineBuilderPtr (*make_cmd_line_builder)();
	// 我看下面这俩各种编译器不用分开
    //string compile_cmd_include_dirs;
    //string link_cmd_lib_dirs;
};

extern CompilerInfo cc_info[];
extern map<string, CC> map_compiler_name2enum;

#endif // COMPILERINFO_H
