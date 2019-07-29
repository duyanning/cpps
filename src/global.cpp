#include "config.h"
#include "global.h"
#include "UserDefinedRule.h"
#include "FileEntity.h"

// 路径相关的变量命名约定：
// xxx_file 文件的相对路径，类型fs::path
// xxx_dir 目录的相对路径，类型fs::path
// xxx_file_path 文件的绝对路径，类型fs::path
// xxx_dir_path 目录的绝对路径，类型fs::path
// xxx_file_name 文件的相对路径或绝对路径，类型string
// xxx_dir_name 目录的相对路径或绝对路径，类型string
// 如果xxx部分足够清晰，可省略file与dir后缀。

// 编译、链接用的命令
string compile_cpp_cmd;
string compile_h_cmd;
string link_cmd;

// 命令行参数对应的变量
bool verbose = false;
bool very_verbose = false;
bool collect_only = false;
bool report_time = false;
bool build_only = false;
bool show_dep_graph = false;
string script_file_name;
string main_file_name;
string class_name;
bool clear_run = false;
string run_by; // exec/system
string compile_by; // gcc/mingw/vc
int config_general_run_by = 1;
string config_general_compile_by = "gcc";
int max_line_scan = -1;              // 最多扫描这么多行，-1代表全部扫描
string output_name;

// 跟据命令行选项跟配置文件得到的信息
string compiler_dir;
string compile_cmd_include_dirs;
string link_cmd_lib_dirs;
string link_cmd_libs;
string exec_cmd_env_vars;
CmdLineBuilderPtr cmd_line_builder = nullptr;


int script_pos; // 脚本在cpps命令行参数中的位置
int script_argc; // 脚本的参数个数

int original_argc;
char** original_argv;


char usage[] = "Usage: cpps [options] <script.cpp> [args]";

po::variables_map vm;


// 扫描源代码搜集到的项目信息
fs::path exe_path;              // 生成的可执行文件的绝对路径
fs::path script_file;           // 命令行上指定的脚本路径（这是个相对路径）
vector<fs::path> sources; // 所有.cpp文件的绝对路径
vector<string> libs; // 源文件中指定的需要链接的库的名字。(就gcc来说，就是链接时命令行上-l之后的部分)
vector<fs::path> headers_to_pc; // 所有需要预编译的头文件的绝对路径
//vector<fs::path> sources_to_pc; // 所有需要预编译的.cpp文件的绝对路径(vc的头文件不能直接编译，必须被#include在一个.cpp文件中编译。)
//map<fs::path, fs::path> source2header_to_pc; // 预编译头文件跟对应的.cpp的对应关系
bool vc_use_pch = false; // vc是否使用预编译头文件。(目前只支持一个预编译头文件)
fs::path vc_h_to_precompile; // vc需要预编译的头文件。等于headers_to_pc[0]
fs::path vc_cpp_to_generate_pch; // vc用于产生预编译头文件的cpp文件
string compiler_specific_extra_compile_flags; // 源文件中指定的，编译时用的其他选项
unordered_map<string, string> compiler_specific_extra_compile_flags_local; // 源文件中指定的，编译时用的其他选项。不同文件可能用不同的编译选项
string compiler_specific_extra_link_flags; // 源文件中指定的，链接时用的其他选项

vector<UserDefinedRule> user_defined_rules; // .cpp文件中指定的，用户自定义规则


//unordered_map<fs::path, FileEntityPtr> map_file_path_to_ptr;
unordered_map<string, FileEntityPtr> map_file_path_to_ptr;