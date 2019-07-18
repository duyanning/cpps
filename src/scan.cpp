#include "config.h"
#include "global.h"
#include "Loggers.h"
#include "helpers.h"

void scan(fs::path src_path)
{
	// 如果已经处理过该文件，就不要再处理，避免循环引用
	if (find(sources.begin(), sources.end(), src_path) != sources.end()) {
		MINILOG(collect_info_logger, src_path << " is already collected.");
		return;
	}

	MINILOG(collect_info_logger, "scanning " << src_path);

	sources.push_back(src_path);

	ifstream in(src_path.string());

	string line;
	regex usingcpp_pat{ R"(^\s*#include\s+"([\w\./]+)\.h"\s+//\s+usingcpp)" };
	regex using_pat{ R"(using\s+([\w\./]+\.(c|cpp|cxx|c\+\+|C|cc|cp|CPP)))" };
	regex linklib_pat{ R"(linklib\s+([\w\-\.]+))" };
	regex precompile_pat{ R"***(^\s*#include\s+"([\w\./]+\.(h|hpp|H|hh))"\s+//\s+precompile)***" };
	string extra_compile_flags_string = cc_info[cc].compiler_name;
	extra_compile_flags_string += R"(-extra-compile-flags:\s+(.*)$)";
	regex extra_compile_flags_pat{ extra_compile_flags_string };
	string extra_link_flags_string = cc_info[cc].compiler_name;
	extra_link_flags_string += R"(-extra-link-flags:\s+(.*)$)";
	regex extra_link_flags_pat{ extra_link_flags_string };
	int n = 0;
	while (getline(in, line)) {
		smatch matches;
		// 搜集引用的.cpp文件
		if (regex_search(line, matches, usingcpp_pat)) {
			string cpp_file_name = matches[1];
			cpp_file_name += ".cpp";
			MINILOG(collect_info_logger, "found: " << cpp_file_name);
			// 一个.cpp文件中引用的另一个.cpp文件，是以相对路径的形式指明的
			fs::path a = src_path;
			a.remove_filename();
			a /= cpp_file_name;
			if (exists(a)) {
				// 递归处理
				scan(a);
			}
			else {
				cout << a << " referenced by " << src_path << " does NOT exsit!" << endl;
				throw 1;
			}
		}
		else if (regex_search(line, matches, using_pat)) {
			MINILOG(collect_info_logger, "found: " << matches[1]);
			// 一个.cpp文件中引用的另一个.cpp文件，是以相对路径的形式指明的
			fs::path a = src_path;
			a.remove_filename();
			a /= matches.str(1);
			if (exists(a)) {
				// 递归处理
				scan(a);
			}
			else {
				cout << a << " referenced by " << src_path << " does NOT exsit!" << endl;
				throw 1;
			}
		}

		// 搜集使用的库名字
		if (regex_search(line, matches, linklib_pat)) {
			MINILOG(collect_info_logger, "found lib: " << matches[1]);
			libs.push_back(matches[1]);
		}

		if (regex_search(line, matches, extra_compile_flags_pat)) {
			MINILOG(collect_info_logger, "found extra compile flags: " << matches[1]);
			extra_compile_flags += " ";
			extra_compile_flags += matches[1];
		}

		if (regex_search(line, matches, extra_link_flags_pat)) {
			MINILOG(collect_info_logger, "found extra link flags: " << matches[1]);
			extra_link_flags += " ";
			extra_link_flags += matches[1];
		}

		// 搜集需要预编译的头文件
		if (regex_search(line, matches, precompile_pat)) {
			MINILOG(collect_info_logger, "found pch: " << matches[1]);
			// 一个.cpp文件要求预编译某个头文件，是以相对路径的形式指明的
			fs::path a = src_path;
			a.remove_filename();
			a /= matches.str(1);
			if (exists(a)) {
				if (find(headers_to_pc.begin(), headers_to_pc.end(), a) != headers_to_pc.end()) {
					MINILOG(collect_info_logger, a << " is already collected.");
					return;
				}
				headers_to_pc.push_back(a);

				if (cc == CC::GCC) {
				}
				else if (cc == CC::VC) {
					sources_to_pc.push_back(src_path);
					source2header_to_pc[src_path] = a;
					if (headers_to_pc.size() >= 2) {
						cout << "at the moment, vcpps supports only one precompiled header, but many found:" << endl;
						for (auto h : headers_to_pc) {
							cout << h.filename() << endl;
						}
						throw 1;
					}

				}
				else {
					assert(false);
				}

			}
			else {
				cout << a << " referenced by " << src_path << " does NOT exsit!" << endl;
				throw 1;
			}
		}


		if (max_line_scan != -1) {
			n++;
			if (n >= max_line_scan)
				break;
		}

	}

	MINILOG(collect_info_logger, "scanning " << src_path << " completed");

}

void collect_info()
{
	// 确定可执行文件的路径
	fs::path script_path = canonical(script_file).make_preferred();
	exe_path = shadow(script_path);
	exe_path += "." + cc_info[cc].compiler_name;
	exe_path += ".exe";

	// 确定所有.cpp文件的路径；确定所有库的名字；确定所有的预编译头文件的路径
	scan(canonical(script_path).make_preferred());
	//    cout << "***" << script_file << endl;
	//    cout << "***" << script_path.make_preferred() << endl;
	//    cout << "***" << canonical(script_path) << endl;

}
