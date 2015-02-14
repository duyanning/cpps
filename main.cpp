#include "std.h"
#include "FileEntity.h"
#include "PhonyEntity.h"
#include "Obj2ExeAction.h"
#include "Cpp2ObjAction.h"
#include "H2GchAction.h"
#include "UpdateDependencyGraphAction.h"
#include "ShebangMagic.h"
#include "GchMagic.h"
#include "helpers.h"
#include "Loggers.h"

fs::path exe_name;
fs::path script_name;

vector<fs::path> sources; // 所有.cpp文件的绝对路径
vector<string> libs; // 库的名称，即链接时命令行上-l之后的部分
vector<fs::path> headers_to_pc; // 所有需要预编译的头文件的绝对路径


void collect_info(fs::path script_name);
int build();
int generate_skeleton_file(string file_name);
fs::path resolve_shebang_wrapper(fs::path wrapper_path);

// 命令行参数对应的变量
bool verbose = false;
bool collect_only = false;
bool show_dep_graph = false;
string src_file;
string skeleton_file;

int run_by = 1; // 0 - system(); 1 - execv()

const int max_num_of_args = 100;
const int max_len_of_arg = 100;
char script_arg_vector[max_num_of_args][max_len_of_arg];
char* script_argv[max_num_of_args];
int script_argc;

char usage[] = "Usage: cpps [options] <script.cpp> [args]";

int main(int argc, char* argv[])
try {
    // 处理命令行参数

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "produce help message")
        ("run-by,r", po::value<int>(&run_by)->default_value(1), "run by 0 - system() or 1 - execv()")
        ("verbose,v", po::bool_switch(&verbose), "be verbose")
        ("dependency,d", po::bool_switch(&show_dep_graph), "show dependency graph")
        ("script", po::value(&src_file), ".cpp file including int main()")
        ("args", po::value<vector<string>>(), "args being passed to the script")
        ("gen,g", po::value(&skeleton_file), "generate script skeleton")
        ("collect,c", po::bool_switch(&collect_only), "only collect info")
        ;

    po::positional_options_description p;
    p.add("script", 1);
    p.add("args", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);    

    if (vm.count("help")) {
        cout << usage << endl;
        cout << desc << "\n";
        return 0;
    }

    if (vm.count("gen")) {
        generate_skeleton_file(skeleton_file);
        return 0;
    }

    if (vm.count("script") == 0) {
        cout << usage << endl;
        return 0;
    }

    if (verbose) {
        build_gch_summay_logger.enable();
        build_exe_summay_logger.enable();
        perm_logger.enable();
    }

    if (collect_only) {
        collect_info_logger.enable();
    }



    script_name = src_file;

    if (!exists(script_name)) {
        cout << "No such file.\n";
        return -1;
    }

    if (extension(script_name) == ".cpps") {
        script_name = resolve_shebang_wrapper(script_name);
        MINILOG(shebang_logger, "resolved to " << script_name);

    }

    int status = 0;
    status = build();
    if (status)
        return status;

    if (run_by == 1) {
        MINILOG0("run using execv()");

        for (int i = 0; i < max_num_of_args; i++) {
            script_argv[i] = script_arg_vector[i];
            script_arg_vector[i][0] = '\0';
        }
        script_argc = 0;

        strcpy(script_argv[0], script_name.c_str());
        script_argc++;

        if (vm.count("args")) {
            for (auto a : vm["args"].as<vector<string>>()) {
                strcpy(script_arg_vector[script_argc++], a.c_str());
            }
        }
        script_argv[script_argc] = 0;

        execv(exe_name.c_str(), script_argv);
    }
    else if (run_by == 0) {
        MINILOG0("run using system()");
        string script_args;
        if (vm.count("args")) {
            for (auto a : vm["args"].as<vector<string>>()) {
                script_args += " '"; // 把脚本的参数用单引号括起来，避免通配符展开。该展开的通配符在解释器执行时已经展开过了
                script_args += a;
                script_args += "'";
            }
        }
        
        string cmd_line= exe_name.string();
        cmd_line += script_args;
        MINILOG0("final cmd line: " << cmd_line);
        // 运行产生的可执行文件
        system(cmd_line.c_str());
    }

    return 0;
}
catch (int) {
    return -1;
}


int build_exe()
{
    // 构建依赖关系图
    FileEntityPtr exe = makeFileEntity(exe_name);
    string lib_options;
    for (auto l : libs) {
        lib_options += " -l";
        lib_options += l;
    }
    exe->addAction(makeObj2ExeAction(lib_options));

    PhonyEntityPtr update_dependency = makePhonyEntity("update dependency graph"); 

    for (auto src_path : sources) {

        // 根据.cpp文件的名字，确定.o文件的名字
        fs::path obj_path = shadow(src_path);
        obj_path += ".o";

        // 
        FileEntityPtr obj = makeFileEntity(obj_path);
        obj->addAction(makeCpp2ObjAction());

        // 可执行文件依赖.o文件
        exe->addPrerequisite(obj);

        // .o文件依赖.cpp文件
        FileEntityPtr src = makeFileEntity(src_path);
        obj->addPrerequisite(src);

        // 根据.o文件的名字，确定.d文件的名字
        fs::path dep_path = obj_path;
        dep_path += ".d";

        if (exists(dep_path)) {
            PhonyEntityPtr obj_update = makePhonyEntity("update for " + obj_path.string());
            obj_update->addAction(makeUpdateDependencyGraphAction(obj));
            update_dependency->addPrerequisite(obj_update);

            FileEntityPtr dep = makeFileEntity(dep_path);
            obj_update->addPrerequisite(dep);
        }


    } // for

    // 根据依赖关系图进行构建

    MINILOGBLK_IF(
        show_dep_graph, dep_graph_logger,
        os << endl;
        update_dependency->show(os);
        os << endl;
        );

    update_dependency->update();

    MINILOGBLK_IF(
        show_dep_graph, dep_graph_logger,
        exe->show(os);
        os << endl;
        );

    exe->update();


    return 0;

}

int build_gch()
{
    if (headers_to_pc.empty())
        return 0;
			
    PhonyEntityPtr all_gch  = makePhonyEntity("generate all gch");
    PhonyEntityPtr update_dependency = makePhonyEntity("update dependency graph");

    for (auto header_path : headers_to_pc) {
        fs::path gch_path = header_path;
        gch_path += ".gch";



        FileEntityPtr gch = makeFileEntity(gch_path);
        gch->addAction(makeH2GchAction());

        // 
        all_gch->addPrerequisite(gch);

        // .gch文件依赖.h文件
        FileEntityPtr header = makeFileEntity(header_path);
        gch->addPrerequisite(header);

        // 根据.gch文件的名字，确定.d文件的名字
        fs::path dep_path = shadow(gch_path);
        dep_path += ".d";

        if (exists(dep_path)) {
            PhonyEntityPtr gch_update = makePhonyEntity("update for " + gch_path.string());
            gch_update->addAction(makeUpdateDependencyGraphAction(gch));
            update_dependency->addPrerequisite(gch_update);

            FileEntityPtr dep = makeFileEntity(dep_path);
            gch_update->addPrerequisite(dep);
        }
    } // for

    // 根据依赖关系图进行构建
    MINILOGBLK_IF(
        show_dep_graph, dep_graph_logger,
        os << endl;
        update_dependency->show(os);
        os << endl;
        );

    update_dependency->update();

    MINILOGBLK_IF(
        show_dep_graph, dep_graph_logger,
        all_gch->show(os);
        os << endl;
        );

    all_gch->update();

    return 0;
}

int build()
{
    if (!is_a_cpp_src(script_name) && !is_a_c_src(script_name)) {
        cout << script_name << " should have a C/C++ suffix." << endl;
        return -1;
    }

    ShebangMagic shebang_magic(script_name.string());

    // 确定可执行文件的名字
    fs::path script_path = canonical(script_name);
    exe_name = shadow(script_path);
    exe_name += ".exe";

    // 确定所有.cpp文件的路径；确定所有库的名字；确定所有的预编译头文件路径
    collect_info(canonical(script_name));
    if (collect_only) {
        return 0;
    }

        
    GchMagic gch_magic(headers_to_pc); 
    build_gch();
    build_exe();

    return 0;
}

void collect_info(fs::path script_name)
{
    // 如果已经处理过该文件，就不要再处理，避免循环引用
    if (find(sources.begin(), sources.end(), script_name) != sources.end()) {
        MINILOG(collect_info_logger, script_name << "is already collected.");
        return;
    }

    MINILOG(collect_info_logger, "collecting " << script_name);

    sources.push_back(script_name);

    ifstream in(script_name.native());

    string line;
    regex using_pat {R"(using\s+([\w\./]+\.(cpp|cxx|c\+\+|C|cc|cp|CPP)))"};
    regex linklib_pat {R"(linklib\s+(\w+))"};
    regex precompile_pat {R"(precompile\s+([\w\./]+\.(h|hpp|H|hh)))"};
    while (getline(in,line)) {
        smatch matches;
        // 搜集引用的.cpp文件
        if (regex_search(line, matches, using_pat)) {
            MINILOG(collect_info_logger, "found " << matches[1]);
            // 一个.cpp文件中引用的另一个.cpp文件，是以相对路径的形式指明的
            fs::path a = script_name;
            a.remove_filename();
            a /= matches.str(1);
            if (exists(a)) {
                // 递归处理
                collect_info(a);
            }
            else {
                cout << a << "referenced by " << script_name << " does NOT exsit!"<< endl;
                throw 1;
            }
        }

        // 搜集使用的库名字
        if (regex_search(line, matches, linklib_pat)) {
            MINILOG(collect_info_logger, "found lib " << matches[1]);
            libs.push_back(matches[1]);
        }

        // 搜集需要预编译的头文件
        if (regex_search(line, matches, precompile_pat)) {
            MINILOG(collect_info_logger, "found pch " << matches[1]);
            // 一个.cpp文件要求预编译某个头文件，是以相对路径的形式指明的
            fs::path a = script_name;
            a.remove_filename();
            a /= matches.str(1);
            if (exists(a)) {
                headers_to_pc.push_back(a);
            }
            else {
                cout << a << "referenced by " << script_name << " does NOT exsit!"<< endl;
                throw 1;
            }
        }


    }

}


int generate_skeleton_file(string file_name)
{
    fs::path p(file_name);
    if (exists(p)) {
        cout << file_name << " already exists." << endl;
        return 0;
    }

    ofstream f(file_name);
    char skeleton[] =
        R"(//#!/usr/bin/env cpps
#include <iostream>

using namespace std;

int main()
{
    cout << "welcome to cpps" << endl;
    return 0;
}
)";

    f << skeleton;
    cout << skeleton_file << " created." << "\n";
    return 1;
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
        throw -1;
    }

    return script_path;
}
