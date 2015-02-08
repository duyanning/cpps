#include "std.h"
#include "FileEntity.h"
#include "PhonyEntity.h"
#include "Obj2ExeAction.h"
#include "Cpp2ObjAction.h"
#include "UpdateDependencyGraphAction.h"

// cmd_line = "g++ -std\\=c++11 -fmax-errors=1"; // -fmax-errors=1让编译器在发现一个错误时就停下来
fs::path build_dir;

void collect_sources_referenced_by(fs::path script_name, vector<fs::path>& sources)
{
    // 如果已经处理过该文件，就不要再处理，避免循环引用
    if (find(sources.begin(), sources.end(), script_name) != sources.end()) {
        MINILOG0(script_name << "is already in list.");
        return;
    }

    MINILOG0("collect sources referenced by " << script_name);

    sources.push_back(script_name);

    ifstream in(script_name.native());

    string line;
    regex pat{R"(using\s+(\w+\.cpp))"};
    while (getline(in,line)) {
        smatch matches;
        if (regex_search(line, matches, pat)) {
            MINILOG0("found " << matches[1] << " in " << script_name);
            // 一个.cpp文件中引用的另一个.cpp文件，是以相对路径的形式指明的
            fs::path a = script_name;
            a.remove_filename();
            a /= matches.str(1);
            if (exists(a)) {
                // 递归处理
                collect_sources_referenced_by(a, sources);
            }
            else {
                cout << a << "referenced by " << script_name << " does NOT exsit!"<< endl;
                throw 1;
            }
        }
    }

}


int main(int argc, char* argv[])
{
    // 处理命令行参数
    bool verbose = false;
    string src_file;

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("compression", po::value<int>(), "set compression level")
        ("verbose,v", po::bool_switch(&verbose), "be verbose")
        ("script,s", po::value(&src_file), ".cpp file including int main()")
        ;

    po::positional_options_description p;
    p.add("script", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);    

    if (vm.count("help")) {
        cout << desc << "\n";
        return 0;
    }

    if (vm.count("script") == 0) {
        cout << "usage: cpps something.cpp" << endl;
        return 0;
    }

    // 解释.cpp脚本
    fs::path script_name(src_file);

    if (!exists(script_name)) {
        cout << "No such file.\n";
        return -1;
    }

    if (script_name.extension() != ".cpp") {
        cout << script_name << " should have a .cpp suffix." << endl;
        return -1;
    }

    // 确保cache(即目录~/.cpps/cache)的存在
    fs::path home_dir(getpwuid(getuid())->pw_dir);
    fs::path cache_dir_name(".cpps/cache");
    fs::path cache_dir = home_dir;
    cache_dir /= cache_dir_name;
    create_directories(cache_dir);

    // 在cache中为该脚本创建build文件夹
    fs::path script_dir = canonical(script_name).parent_path().relative_path();
    build_dir = cache_dir;
    build_dir /= script_dir;
    create_directories(build_dir);

    // 确定可执行文件的名字
    fs::path exe_name = build_dir;
    exe_name /= script_name.stem();
    MINILOG0("exe: " << exe_name.string());

    // 确定所有.cpp文件的名字，如果某个文件不存在，则结束。
    vector<fs::path> sources; // 绝对路径
    try {
        collect_sources_referenced_by(canonical(script_name), sources);
    }
    catch (int) {
        return -1;
    }


    FileEntityPtr exe = makeFileEntity(exe_name.filename().string(), exe_name);
    exe->addAction(makeObj2ExeAction());

    PhonyEntityPtr update_dependency = makePhonyEntity("update dependency graph"); 

    for (auto src_path : sources) {

        // 根据.cpp文件的名字，确定.o文件的名字
        fs::path obj_path = build_dir;
        obj_path /= src_path.stem();
        obj_path += ".o";

        // 
        FileEntityPtr obj{new FileEntity(obj_path.filename().string(), obj_path)};
        obj->addAction(ActionPtr(new Cpp2ObjAction));

        // 可执行文件依赖.o文件
        exe->addPrerequisite(obj);

        // .o文件依赖.cpp文件
        FileEntityPtr src{new FileEntity(src_path.filename().string(), src_path)};
        obj->addPrerequisite(src);

        // 根据.cpp文件的名字，确定.dep文件的名字
        fs::path dep_path = build_dir;
        dep_path /= src_path.stem();
        dep_path += ".d";

        if (exists(dep_path)) {
            PhonyEntityPtr obj_update {new PhonyEntity("update for " + obj_path.string())};
            obj_update->addAction(PUpdateDependencyGraphAction(new UpdateDependencyGraphAction(obj)));
            update_dependency->addPrerequisite(obj_update);

            FileEntityPtr dep {new FileEntity(dep_path.filename().string(), dep_path)};
            obj_update->addPrerequisite(dep);
        }


    } // for

    // 构建
    try {
        update_dependency->show();
        //update_dependency->update();
        exe->show();
        //exe->update();
        return 0;
    }
    catch (int) {
        return -1;
    }

    // 运行
    system(exe_name.c_str());

    return 0;
}
