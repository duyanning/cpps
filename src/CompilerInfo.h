#ifndef COMPILERINFO_H
#define COMPILERINFO_H

// 目前支持的底层编译器
enum CC {
	GCC,
	VC
};

extern CC cc;

struct CompilerInfo {
    const string compiler_name;
	const char* obj_ext;
	const char* pch_ext;
    string cmd_line_include_dirs;
    string cmd_line_lib_dirs;
};

extern CompilerInfo cc_info[];


#endif // COMPILERINFO_H
