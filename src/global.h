#ifndef GLOBAL_H
#define GLOBAL_H

#include "CompilerInfo.h"
#include "CmdLineBuilder.h"
#include "UserDefinedRule.h"
#include "FileEntity.h"

void parse(int argc, char* argv[]);
void collect();
bool build_exe();
bool build();
void run();
void generate_main_file(string main_file__name);
void generate_class_files(string class_name);


extern string compile_cpp_cmd;
extern string compile_h_cmd;
extern string link_cmd;
extern fs::path exe_path;
extern fs::path script_file;
extern vector<fs::path> sources;
extern vector<string> libs;
extern vector<fs::path> headers_to_pc;
extern vector<fs::path> sources_to_pc;
extern map<fs::path, fs::path> source2header_to_pc;
extern bool vc_use_pch;
extern fs::path vc_h_to_precompile;
extern fs::path vc_cpp_to_generate_pch;
//extern string compiler_specific_extra_compile_flags;
//extern unordered_map<fs::path, string> compiler_specific_extra_compile_flags;
extern unordered_map<string, string> compiler_specific_extra_compile_flags;
extern string compiler_specific_extra_link_flags;
extern vector<UserDefinedRule> user_defined_rules;


//extern unordered_map<fs::path, FileEntityPtr> map_file_path_to_ptr;
extern unordered_map<string, FileEntityPtr> map_file_path_to_ptr;


extern string compiler_dir;
extern string compile_cmd_include_dirs;
extern string link_cmd_lib_dirs;
extern string link_cmd_libs;
extern string exec_cmd_env_vars;
extern CmdLineBuilderPtr cmd_line_builder;
extern bool verbose;
extern bool very_verbose;
extern bool collect_only;
extern bool build_only;
extern bool report_time;
extern bool show_dep_graph;
extern string script_file_name;
extern string main_file_name;
extern string class_name;
extern bool clear_run;
extern string run_by;
extern string compile_by;
extern int config_general_run_by;
extern string config_general_compile_by;
extern int max_line_scan;
extern string output_name;

void parse(int argc, char* argv[]);
void collect();
bool build_exe();
bool build();
void run();
void generate_main_file(string main_file__name);
void generate_class_files(string class_name);
fs::path resolve_shebang_wrapper(fs::path wrapper_path);

extern char usage[];
extern po::variables_map vm;
extern int script_pos;
extern int script_argc;
extern int original_argc;
extern char** original_argv;


#endif // GLOBAL_H
