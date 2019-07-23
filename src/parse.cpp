#include "config.h"
#include "global.h"
#include "helpers.h"
#include "Loggers.h"

fs::path resolve_shebang_wrapper(fs::path wrapper_path);

void parse(int argc, char* argv[])
{
	//cout << argv[0] << endl;
	original_argc = argc;
	original_argv = argv;
	//cout << "original_argc: " << original_argc << endl;
	//cout << "original_argv: " << original_argv << endl;
 //   {
 //       cout << "argc: " << argc << endl;
 //       for (int i = 0; i < argc; i++) {
 //           cout << "arg[" << i << "]: " << argv[i] << endl;
 //       }
 //       assert(argv[argc] == nullptr);
 //       cout << "================================================================" << endl;

 //   }

	// 确定脚本在命令行上的位置script_pos
	// 调整cpps的argc，不让boost::program_options看到属于脚本的命令行选项与参数
	int argc_excluding_script_arguments = argc;
	for (int i = 0; i < argc; i++) {
		if (is_a_cpp_src(argv[i])) {
			script_pos = i;
			script_argc = argc - i;
			argc_excluding_script_arguments = i + 1;
			//cout << "script_pos:" << script_pos << endl;
			//cout << "argc_script:" << argc_script << endl;
			break;
		}
	}


	// 命令行上的各种选项
	po::options_description info_opts("Information options");
	info_opts.add_options()
		("help,h", "produce help message")
		("version", "show version message")
		("verbose,v", po::bool_switch(&verbose), "be verbose")
		("very-verbose", po::bool_switch(&very_verbose), "be very verbose")
		("dependency,d", po::bool_switch(&show_dep_graph), "show dependency graph")
		("collect", po::bool_switch(&collect_only), "only show information collected")
		;

	po::options_description build_opts("Build options");
	build_opts.add_options()
		("build", po::bool_switch(&build_only), "only build")
		("output,o", po::value(&output_name), "output resulting exe as named")
		("clear", po::bool_switch(&clear_run), "run within a clear environment")
		("max-line-scan", po::value<int>(&max_line_scan), "scan up to N lines")
		;

	po::options_description run_opts("Run options");
	run_opts.add_options()
		("run-by,r", po::value<string>(), "run resulting app using: exec, system")
		("compile-by,c", po::value<string>(), "compile script using: gcc, mingw, vc")
		;

	po::options_description generation_opts("Generation options");
	generation_opts.add_options()
		("generate,g", po::value(&main_file_name), "'cpps -g hello.cpp' generate a helloworld program")
		("class", po::value(&class_name), "'cpps --class Car' generate Car.cpp and Car.h")
		;

	po::options_description config_opts("Configuration options");
	config_opts.add_options()
		("include-dir,I", po::value<vector<string>>(), "add a directory to be searched for header files")
		("lib-dir,L", po::value<vector<string>>(), "add a directory to be searched for libs")
		("dll-dir", po::value<vector<string>>(), "add a directory to be searched for dlls")
		;

	po::options_description hidden_opts("Hidden options");
	hidden_opts.add_options()
		("script", po::value(&script_file_name), ".cpp file including int main()")
		("args", po::value<vector<string>>(), "args being passed to the script")
		;

	po::options_description cmdline_options; // 用于解析命令的选项
	cmdline_options.add(info_opts).add(build_opts).add(run_opts).add(generation_opts).add(config_opts).add(hidden_opts);

	po::options_description visible_options; // 呈现给用户的选项
	visible_options.add(info_opts).add(build_opts).add(run_opts).add(generation_opts).add(config_opts);

	po::positional_options_description p;
	p.add("script", 1);
	p.add("args", -1);

	// 解析命令行
	po::store(po::command_line_parser(argc_excluding_script_arguments, argv).options(cmdline_options).positional(p).allow_unregistered().run(), vm);

	// 配置文件中的各种选项
	po::options_description config_file_opts("config.txt options");
	config_file_opts.add_options()
		//("general.run-by", po::value<int>(&config_general_run_by)->default_value(1), "run using: 0 - system(), 1 - execv()")
		("general.compile-by", po::value<string>(&config_general_compile_by)->default_value("gcc"), "compile using: gcc, mingw, vc")
		("gcc.compiler-dir", po::value<string>(), "directory where gcc compiler resides")
		("gcc.include-dir", po::value<vector<string>>(), "add a directory to be searched for header files")
		("gcc.lib-dir", po::value<vector<string>>(), "add a directory to be searched for libs")
		("gcc.dll-dir", po::value<vector<string>>(), "add a directory to be searched for dlls")
		("mingw.compiler-dir", po::value<string>(), "directory where mingw compiler resides")
		("mingw.include-dir", po::value<vector<string>>(), "add a directory to be searched for header files")
		("mingw.lib-dir", po::value<vector<string>>(), "add a directory to be searched for libs")
		("mingw.dll-dir", po::value<vector<string>>(), "add a directory to be searched for dlls")
		("vc.compiler-dir", po::value<string>(), "directory where vc compiler resides")
		("vc.include-dir", po::value<vector<string>>(), "add a directory to be searched for header files")
		("vc.lib-dir", po::value<vector<string>>(), "add a directory to be searched for libs")
		("vc.dll-dir", po::value<vector<string>>(), "add a directory to be searched for dlls")
		;

	// 读取配置文件
	fs::path cfg_path = get_home();
	cfg_path /= ".cpps/config.txt";
	ifstream ifs(cfg_path.string());
	if (ifs) {
		try {
			po::store(parse_config_file(ifs, config_file_opts), vm);
		}
		catch (const po::error& ex) {
			std::cerr << ex.what() << '\n';
			cerr << "error in config.txt" << endl;
			throw 0;
		}
	}

	po::notify(vm);

	// 处理命令行上和配置文件中的选项
	if (vm.count("compile-by")) {
		compile_by = vm["compile-by"].as<string>();
	}
	else if (vm.count("general.compile-by")) {
		compile_by = vm["general.compile-by"].as<string>();
	}
	else {
#if defined(_MSC_VER)
		compile_by = "vc";
#elif defined(__MINGW32__)
		compile_by = "mingw";
#else
		compile_by = "gcc";
#endif
	}

	auto it = map_compiler_name2enum.find(compile_by);
	if (it != map_compiler_name2enum.end()) {
		cc = it->second;
	}
	else {
		cout << "unsupported compiler: " << compile_by << endl;
		throw 0;
	}

	if (vm.count("run-by")) {
		run_by = vm["run-by"].as<string>();
	}
	else {
		run_by = "exec";
	}

	if (vm.count("help")) {
		cout << usage << endl;
		cout << visible_options << "\n";
		throw 0;
	}

	if (vm.count("version")) {
		cout << "use: " << compile_by << endl;
		throw 0;
	}

	if (vm.count("generate")) {
		generate_main_file(main_file_name);
		throw 0;
	}

	if (vm.count("class")) {
		generate_class_files(class_name);
		throw 0;
	}

	if (vm.count("script") == 0) {
		cout << usage << endl;
		throw 0;
	}

	if (verbose) {
		build_gch_summay_logger.enable();
		build_exe_summay_logger.enable();
		perm_logger.enable();
	}

	if (very_verbose) {
		build_gch_summay_logger.enable();
		build_gch_detail_logger.enable();
		build_exe_summay_logger.enable();
		build_exe_detail_logger.enable();
		perm_logger.enable();
	}

	if (collect_only) {
		collect_info_logger.enable();
	}

	script_file = script_file_name;

	if (!exists(script_file)) {
		cout << "No such file.\n";
		throw 1;
	}

	if (extension(script_file) == ".cpps") {
		script_file = resolve_shebang_wrapper(script_file);
		MINILOG(shebang_logger, "resolved to " << script_file);
	}

	if (!is_a_cpp_src(script_file) && !is_a_c_src(script_file)) {
		cout << script_file << " should have a C/C++ suffix." << endl;
		throw 1;
	}

	// 起个简单点的名字
	compile_cpp_cmd = cc_info[cc].compile_cpp_cmd;
	compile_h_cmd = cc_info[cc].compile_h_cmd;
	compile_h_cmd = cc_info[cc].compile_h_cmd;
	link_cmd = cc_info[cc].link_cmd;
	cmd_line_builder = cc_info[cc].make_cmd_line_builder();

	// 命令行上指定的头文件目录
	if (vm.count("include-dir")) {
		cmd_line_builder->add_include_dirs(compile_cmd_include_dirs, vm["include-dir"].as<vector<string>>());
	}
	// 配置文件中指定的头文件目录
	string config_file_include_dir = cc_info[cc].compiler_name + ".include-dir";
	if (vm.count(config_file_include_dir)) {
		cmd_line_builder->add_include_dirs(compile_cmd_include_dirs, vm[config_file_include_dir].as<vector<string>>());
	}

	// 命令行上指定的库文件目录
	if (vm.count("lib-dir")) {
		cmd_line_builder->add_lib_dirs(link_cmd_lib_dirs, vm["lib-dir"].as<vector<string>>());
	}
	// 配置文件中指定的库文件目录
	string config_file_lib_dir = cc_info[cc].compiler_name + ".lib-dir";
	if (vm.count(config_file_lib_dir)) {
		cmd_line_builder->add_lib_dirs(link_cmd_lib_dirs, vm[config_file_lib_dir].as<vector<string>>());
	}

	// 命令行上指定的dll目录
	if (vm.count("dll-dir")) {
		cmd_line_builder->add_dll_dirs(exec_cmd_env_vars, vm["dll-dir"].as<vector<string>>());
	}
	// 配置文件中指定的dll目录
	string config_file_dll_dir = cc_info[cc].compiler_name + ".dll-dir";
	if (vm.count(config_file_dll_dir)) {
		cmd_line_builder->add_dll_dirs(exec_cmd_env_vars, vm[config_file_dll_dir].as<vector<string>>());
	}

	// 将编译器所在目录加入PATH环境变量，以便cpps调用
	string config_file_compiler_dir = cc_info[cc].compiler_name + ".compiler-dir";
	if (vm.count(config_file_compiler_dir)) {
		compiler_dir = vm[config_file_compiler_dir].as<string>();
		string env_path_value = "";
		env_path_value += compiler_dir;
		env_path_value += ";";
		env_path_value += getenv("PATH");
		put_env("PATH", env_path_value.c_str());
	}

}

fs::path resolve_shebang_wrapper(fs::path wrapper_path)
{
	// 得到实际脚本的路径
	ifstream in(wrapper_path.string());
	string firstline, secondline;
	getline(in, firstline);
	getline(in, secondline);
	fs::path script_path(secondline);


	if (!script_path.is_absolute()) { // 如果该路径是个相对路径，根据wrapper的路径进行拼装
		script_path = wrapper_path.parent_path();;
		script_path /= secondline;
	}

	if (!exists(script_path)) {
		cout << script_path << " referenced by " << wrapper_path << " does NOT exist." << endl;
		throw - 1;
	}

	return script_path;
}
