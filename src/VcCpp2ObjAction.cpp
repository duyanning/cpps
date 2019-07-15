#include "config.h"
#include <regex>
#include "VcCpp2ObjAction.h"
//#include "VulnerableFileEntity.h"
#include "FileEntity.h"
#include "Loggers.h"
#include "helpers.h"
#include "global.h"

bool VcCpp2ObjAction::execute(const DepInfo& info)
{
    // 一个.o依赖于一个.cpp跟若干个.h
    // .h更新失败(比如.h不存在)无所谓，因为有可能目前已经不依赖该.h
    // 但.cpp不存在将无法产生.o
    if (!info.failed.empty()) {
        for (auto pre : info.failed) {
            FileEntityPtr fe = static_pointer_cast<FileEntity>(pre);
            if (is_a_cpp_src(fe->path())) return false;
        }
    }

    // 构造命令行
    FileEntityPtr cpp = static_pointer_cast<FileEntity>(info.all[0]);
    fs::path cpp_path = cpp->path();

    //VulnerableFileEntityPtr obj = static_pointer_cast<VulnerableFileEntity>(info.target);
    FileEntityPtr obj = static_pointer_cast<FileEntity>(info.target);
    fs::path obj_path = obj->path();

	fs::path showIncludes_path = obj_path;
	showIncludes_path += ".showIncludes";

    fs::path dep_path = obj_path;
    dep_path += ".d";

    // fs::path birthcert_path = obj_path;
    // birthcert_path += ".birthcert";

    string cmd = vc_compile_cpp_cmd;

    cmd += " ";
    cmd += cc_info[cc].cmd_line_include_dirs;
        
	cmd += m_additional_options;

    cmd += " /Fo:";

    cmd += " ";
    cmd += obj_path.string();

    cmd += " ";
    cmd += cpp_path.string();

    //cmd += " -fpch-deps -MMD -MF " + dep_path.string();
	cmd += R"( /showIncludes | sed -r -e "/Note: including file:[[:space:]]+C:\\\\Program Files/d" -e "/Note: including file:/w)";
	cmd += " ";
	string showIncludes_path_string = showIncludes_path.string();
	showIncludes_path_string = regex_replace(showIncludes_path_string, std::regex(R"(\\)"), R"(\\\\)"); // \换为\\\\，但因为前一个是正则，所以得转义，后边那个不用。
	cmd += showIncludes_path_string;
	cmd += R"(")";
	cmd += R"( -e "/Note: including file:/d")";
	//cmd += R"( -e "/^[[:space:]]*$/d")";
	cmd += R"( -e "/^)";
	cmd += cpp_path.filename().string(); // cl每编译一个文件之前，会输出该文件的名字，无法关闭，只能用sed剔除
	cmd += R"($/d")";
	cmd += R"( | finderror)";

    MINILOG(build_exe_summay_logger, "compiling " << cpp_path.filename().string());
    MINILOG(build_exe_detail_logger, cmd);

    // 确保目录存在
    create_directories(obj_path.parent_path());

    // 产生.o文件和.d文件
    int gcc_status;
    gcc_status = system(cmd.c_str());
    // 函数system给出的就是gcc的返回值
    // 在shell下执行gcc后，可用echo $?来查看该返回值
    // 有编译错误时，该返回值为1，无错误时为0
    // 即便有警告，仍旧是0。
    //cout << "cpp2obj:" << gcc_status << endl;
	if (gcc_status) {            // 非0就是代表失败了
		return false;
	}

	// 从.showIncludes文件生成.d文件
	ifstream ifs_showIncludes(showIncludes_path.string());
	assert(ifs_showIncludes);
	unordered_set<string> path_set; // 利用set排除重复路径
	while (ifs_showIncludes) {
		// Note: including file: C:\Users\duyanning\Desktop\test\a/f.h
		string t1, t2, t3;
		ifs_showIncludes >> t1 >> t2 >> t3; // skip
		string a_path;
		ifs_showIncludes >> a_path;
		//cout << "a path: " << a_path << endl;
		if (a_path == "")
			break;
		a_path = regex_replace(a_path, std::regex(R"(/)"), R"(\)");
		path_set.insert(a_path);
	}

	//for (auto x : path_set) {
	//	cout << "a path: " << x << endl;
	//}

	// 被预编译的头文件没出现在/showIncludes中。我们自己加上
	path_set.insert(m_h_path.string());

	ofstream ofs_d{ dep_path.string() };
	ofs_d << obj_path.string() << ": \\" << endl;
	ofs_d << " " << cpp_path.string() << " \\" << endl;
	auto total_headers = path_set.size();
	for (auto header : path_set) {
		ofs_d << " " << header;
		total_headers--;
		if (total_headers > 0) {
			ofs_d << " \\";
		}
		ofs_d << endl;
	}






    // 产生出生证明文件（gcc编译时，如果遇到#include的头文件不存在，就算fatal error，也不会生成.d文件）
	// 从本次运行新生成的.d生成.birthcert，可以保证二者一致。依赖图反映的是老.d
    obj->generate_birth_cert(dep_path);
    //obj->generate_birth_cert();

    return true;

}
