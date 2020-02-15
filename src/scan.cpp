#include "config.h"
#include "global.h"
#include "Loggers.h"
#include "helpers.h"
#include "InfoPackageScanned.h"
#include "sha1.hpp"
#include "UserDefinedRule.h"

#ifdef HAVE_REGEX
using std::regex;
using std::smatch;
#else
using boost::regex;
using boost::smatch;
#endif

void add_libs_from_string(vector<string>& libs_vector, string libs_string)
{
    vector<string> parts;
    std::string delimiters(" "); // 支持多个分界符
    boost::split(parts, libs_string, boost::is_any_of(delimiters), boost::token_compress_on); // token_compress_on是为了将单词之间的多个空格视为一个，但最后一个单词之后的空格还是会导致问题，只有在正则表达式上下功夫了
    //cout << "number: " << parts.size() << endl;
    //for (auto a : parts) {
    //    cout << "---> " << a << " " << a.size() << endl;
    //}

    copy(parts.begin(), parts.end(), back_inserter(libs_vector));
}

void add_include_dir(fs::path including_src, string dir, InfoPackageScanned& pack)
{
    fs::path a = including_src;
    a.remove_filename();

    expand_variable(dir, a);
    fs::path path{ dir };

    if (path.is_absolute()) {
        pack.include_dirs.push_back(path);
    }
    else {
        a /= dir;
        pack.include_dirs.push_back(a);
    }
}

void add_src(fs::path referencing_src, string referenced_name, bool check_existence, InfoPackageScanned& pack)
{
    fs::path a = referencing_src;
    a.remove_filename();

    expand_variable(referenced_name, a);
    fs::path path{ referenced_name };

    if (path.is_absolute()) {
        pack.referenced_sources.push_back(path);
        if (!check_existence) {
            pack.generated_files.push_back(path);
        }

    }
    else {
        a /= referenced_name;
        pack.referenced_sources.push_back(a);
        if (!check_existence) {
            pack.generated_files.push_back(a);
        }

    }

}

void scan(fs::path src_path, InfoPackageScanned& pack)
{
    pack.cpp_sig = SHA1::from_file(src_path.string());

	MINILOG(build_exe_summay_logger, "scanning " << src_path.filename().string());

	ifstream in(src_path.string());

	string line;

    // 如果cpps在运行时崩溃，很有可能是此处正则表达式有问题，往往是R"()"少了最右边的那个括号

    regex usingcpp_pat{ R"(^\s*#include\s+"([\w\./]+)\.h"\s+//\s+usingcpp(\s+(nocheck))?)" };
    regex using_pat{ R"(using(\s+(nocheck)\s+|\s+)([\w\./$()]+\.(cpp|cxx|c\+\+|cc|c)))" }; // | 或的顺序还挺重要，把长的排前边。免得前缀就匹配。
    regex linklib_pat{ R"(//\s+linklib\s+(.+\w))" }; // linklib 后边可以跟多个库的名字，用空格分隔，库名字既可以带扩展名，也可以不带。
    regex include_dir_pat{ R"(//\s+include-dir\s+([\w$()/]+))" }; // include-dir后边可以跟一个目录

	string compiler_specific_linklib_string = R"(//\s+)" + cc_info[cc].compiler_name;
    compiler_specific_linklib_string += R"(-linklib\s+(.+\w))";
	regex compiler_specific_linklib_pat{ compiler_specific_linklib_string };

	// 下面这行，前后三个*号，不是正则的一部分，而是c++ raw-string的自定义delimiter
	// https://stackoverflow.com/questions/49416631/escape-r-in-a-raw-string-in-c
	regex precompile_pat{ R"***(^\s*#include\s+"([\w\./]+\.(h|hpp|H|hh))"\s+//\s+precompile)***" };

	string compiler_specific_extra_compile_flags_string = R"(//\s+)" + cc_info[cc].compiler_name;
	compiler_specific_extra_compile_flags_string += R"(-extra-compile-flags:\s+(.*)$)";
	regex compiler_specific_extra_compile_flags_pat{ compiler_specific_extra_compile_flags_string };

    string compiler_specific_extra_compile_flags_local_string = R"(//\s+)" + cc_info[cc].compiler_name;
    compiler_specific_extra_compile_flags_local_string += R"(-extra-compile-flags(local):\s+(.*)$)";
    regex compiler_specific_extra_compile_flags_local_pat{ compiler_specific_extra_compile_flags_string };

	string compiler_specific_extra_link_flags_string = R"(//\s+)" + cc_info[cc].compiler_name;
	compiler_specific_extra_link_flags_string += R"(-extra-link-flags:\s+(.*)$)";
	regex compiler_specific_extra_link_flags_pat{ compiler_specific_extra_link_flags_string };

    // 想要新增的指令
    // cpps-advanced-features on|off     高级特性打开后，才会扫描一些指令，为的是加快扫描速度
    // cpps-before-compile shell命令         在编译本.cpp之前执行的操作
    // cpps-after-compile shell命令        在编译本.cpp之后执行的操作
    // cpps-before-link shell命令            在链接之前执行的操作
    // cpps-after-link shell命令           在链接之后执行的操作

    regex user_defined_rule_pat{ R"(//\scpps-make\s+(.+\w))" }; // 单行
    regex user_defined_rule_multi_lines_pat{R"(/\* cpps-make)"};

	int n = 0;
	while (getline(in, line)) {
		smatch matches;

        //cout << line << endl;

		// 搜集引用的.cpp文件
		if (regex_search(line, matches, usingcpp_pat)) {
            // !!!!!!! 下面这段注释不要删，留着方便调试
            //cout << matches.size() << endl;
            //for (size_t i = 0; i < matches.size(); i++) {
            //    cout << surround(matches[i]) << endl;
            //}
            string filename = matches[1];
            string nocheck = matches[3];

			filename += ".cpp";
			MINILOG(collect_info_detail_logger, "found: " << filename);
			// 一个.cpp文件中引用的另一个.cpp文件，是以相对路径的形式指明的
            add_src(src_path, filename, nocheck != "nocheck", pack);
			//fs::path a = src_path;
			//a.remove_filename();
			//a /= filename;
   //         pack.referenced_sources.push_back(a);
		}
		else if (regex_search(line, matches, using_pat)) {
			MINILOG(collect_info_detail_logger, "found: " << matches[0]);
            // !!!!!!! 下面这段注释不要删，留着方便调试
            //cout << matches.size() << endl;
            //for (size_t i = 0; i < matches.size(); i++) {
            //    cout << surround(matches[i]) << endl;
            //}
            string nocheck = matches[2];
            string filename = matches[3];
            //exit(0);
			// 一个.cpp文件中引用的另一个.cpp文件，是以相对路径的形式指明的
            add_src(src_path, filename, nocheck != "nocheck", pack);
			//fs::path a = src_path;
			//a.remove_filename();
			//a /= filename;
   //         pack.referenced_sources.push_back(a);
   //         if (nocheck == "nocheck") {
   //             pack.generated_files.push_back(a);
   //         }
		}

		// 搜集使用的库名字
        //cout << "line: " << line << endl;
		if (regex_search(line, matches, linklib_pat)) {
			MINILOG(collect_info_detail_logger, "found lib: " << matches[1]);
			//pack.referenced_libs.push_back(matches[1]);
            //cout << "found lib: " << matches[1] << endl;
            add_libs_from_string(pack.referenced_libs, matches[1]);
		}

		if (regex_search(line, matches, compiler_specific_linklib_pat)) {
			MINILOG(collect_info_detail_logger, "found lib: " << matches[1]);
			//pack.referenced_libs.push_back(matches[1]);
            add_libs_from_string(pack.referenced_libs, matches[1]);
		}

		if (regex_search(line, matches, compiler_specific_extra_compile_flags_pat)) {
            MINILOG(collect_info_detail_logger, "found extra compile flags: " << matches[1]);
            string flags = " ";
            flags += matches[1];
            pack.referenced_compiler_specific_extra_compile_flags += flags;
		}

        if (regex_search(line, matches, compiler_specific_extra_compile_flags_local_pat)) {
            MINILOG(collect_info_detail_logger, "found extra compile flags: " << matches[1]);
            string flags = " ";
            flags += matches[1];
            pack.referenced_compiler_specific_extra_compile_flags_local[src_path.string()] += flags;
        }

        if (regex_search(line, matches, compiler_specific_extra_link_flags_pat)) {
            MINILOG(collect_info_detail_logger, "found extra link flags: " << matches[1]);
            string flags = " ";
            flags += matches[1];
            pack.referenced_compiler_specific_extra_link_flags += flags;
        }

        if (regex_search(line, matches, include_dir_pat)) {
            MINILOG(collect_info_detail_logger, "found include-dir: " << matches[1]);
            string dir = matches[1];
            add_include_dir(src_path, dir, pack);
        }

		// 搜集需要预编译的头文件
		if (regex_search(line, matches, precompile_pat)) {
			MINILOG(collect_info_detail_logger, "found pch: " << matches[1]);
			// 一个.cpp文件要求预编译某个头文件，是以相对路径的形式指明的
			fs::path a = src_path;
			a.remove_filename();
			a /= matches.str(1);
            pack.referenced_headers_to_pc.push_back(a);
            if (cc == CC::VC) {
                pack.referenced_vc_use_pch = true;
                pack.referenced_vc_h_to_precompile = a;
                pack.referenced_vc_cpp_to_generate_pch = src_path;
            }
		}

        // 搜集用户自定义规则
        if (regex_search(line, matches, user_defined_rule_pat)) {
            //cout << matches[1] << endl;
            process_user_defined_rule(src_path, matches[1], pack);

        }
        else if (regex_search(line, matches, user_defined_rule_multi_lines_pat)) {
            //cout << matches[1] << endl;
            string dependency_relationship;
            getline(in, dependency_relationship);
            
            vector<string> commands;
            string c;
            getline(in, c);
            boost::algorithm::trim(c);
            while (c != "*/") {
                commands.push_back(c);
                getline(in, c);
                boost::algorithm::trim(c);
            }
            process_user_defined_rule_multi_lines(src_path, dependency_relationship, commands, pack);

        }

		if (max_line_scan != -1) {
			n++;
			if (n >= max_line_scan)
				break;
		}

	}

}

bool need_check(fs::path p, InfoPackageScanned& pack)
{
    auto it = find(pack.generated_files.begin(), pack.generated_files.end(), p);
    if (it != pack.generated_files.end()) // 是产生的文件，就不需要检查
        return false;
    return true;
}

void check_referenced_file(fs::path src_path, InfoPackageScanned& pack)
{
    for (auto p : pack.referenced_sources) {
        if (!exists(p) && need_check(p, pack)) {
            cout << p << " referenced by " << src_path << " does NOT exsit!" << endl;
            throw 1;
        }
    }

    for (auto p : pack.referenced_headers_to_pc) {
        if (!exists(p) && need_check(p, pack)) {
            cout << p << " referenced by " << src_path << " does NOT exsit!" << endl;
            throw 1;
        }
    }

}

// 将从一个.cpp文件中搜集得到的信息，汇总到全局信息中
void merge(const InfoPackageScanned& pack)
{
    copy(pack.referenced_libs.begin(), pack.referenced_libs.end(), std::back_inserter(libs));
    compiler_specific_extra_compile_flags += pack.referenced_compiler_specific_extra_compile_flags;
    compiler_specific_extra_compile_flags_local.insert(pack.referenced_compiler_specific_extra_compile_flags_local.begin(), pack.referenced_compiler_specific_extra_compile_flags_local.end());
    compiler_specific_extra_link_flags += pack.referenced_compiler_specific_extra_link_flags;
    //copy(pack.referenced_headers_to_pc.begin(), pack.referenced_headers_to_pc.end(), std::back_inserter(headers_to_pc));
    for (auto p : pack.referenced_headers_to_pc) {
        if (find(headers_to_pc.begin(), headers_to_pc.end(), p) == headers_to_pc.end()) {
            headers_to_pc.push_back(p);
        }
    }
    copy(pack.include_dirs.begin(), pack.include_dirs.end(), std::back_inserter(include_dirs));

    vc_use_pch = pack.referenced_vc_use_pch;
    vc_h_to_precompile = pack.referenced_vc_h_to_precompile;
    vc_cpp_to_generate_pch = pack.referenced_vc_cpp_to_generate_pch;

    copy(pack.user_defined_rules.begin(), pack.user_defined_rules.end(), std::back_inserter(
        user_defined_rules));

}

void collect_source(fs::path src_path)
{
    // 如果已经采集过该文件，就不要再采集，避免循环引用
    if (find(sources.begin(), sources.end(), src_path) != sources.end()) {
        MINILOG(collect_info_summary_logger, src_path << " is already collected.");
        return;
    }

    boost::timer::cpu_timer timer;

    MINILOG(collect_info_summary_logger, "collecting " << src_path.filename().string());

    sources.push_back(src_path);


    InfoPackageScanned pack;
    // load or scan
    fs::path pack_path = shadow(src_path);
    pack_path += ".scan";

    if (clear_run) {
        safe_remove(pack_path);
    }

    bool needScan = true;

    if (exists(pack_path)) {
        //cout << pack_path << " pack exists" << endl;
        ifstream ifs(pack_path.string());
        assert(ifs);

        boost::archive::text_iarchive ia(ifs);

        ia >> pack;

        // 如果.cpp在上一次扫描生成信息包后并未发生改变，那就不用重新扫描了
        string cpp_sig = SHA1::from_file(src_path.string());
        if (pack.cpp_sig == cpp_sig) {
            needScan = false;
        }
    }

    if (needScan) {
        //cout << pack_path << " pack NOT exists" << endl;
        // todo: pack可能之前load了东西，scan之前要清除掉。
        pack.clear();
        scan(src_path, pack);

        // 确保目录存在
        create_directories(pack_path.parent_path());

        //cout << pack_path << " 2pack NOT exists" << endl;
        ofstream ofs(pack_path.string());
        assert(ofs);
        boost::archive::text_oarchive oa(ofs);

        oa << pack;

    }
    //else {
    //    cout << "loading pack of " << src_path.filename().string() << endl;
    //}

    // 检查.cpp中引用的文件是否存在
    check_referenced_file(src_path, pack);

    // 将采集到的部分信息合并到整体中
    merge(pack);

    MINILOG(build_exe_timer_logger, timer.format(boost::timer::default_places, "%ws") << " collecting " << src_path.filename());

    // 采集引用的.cpp文件
    for (auto a : pack.referenced_sources) {
        collect_source(a);
    }

}

void collect()
{
	// 确定可执行文件的路径
	fs::path script_path = canonical(script_file).make_preferred();
	exe_path = shadow(script_path);
	exe_path += "." + cc_info[cc].compiler_name;
	exe_path += ".exe";

	// 确定所有.cpp文件的路径；确定所有库的名字；确定所有的预编译头文件的路径
	collect_source(canonical(script_path).make_preferred());
	//    cout << "***" << script_file << endl;
	//    cout << "***" << script_path.make_preferred() << endl;
	//    cout << "***" << canonical(script_path) << endl;

}
