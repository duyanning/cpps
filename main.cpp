#include "std.h"
#include "FileEntity.h"
#include "PhonyEntity.h"
#include "Obj2ExeAction.h"
#include "Cpp2ObjAction.h"
#include "UpdateDependencyGraphAction.h"

fs::path build_dir;

void collect_sources_referenced_by(fs::path script_name, vector<fs::path>& sources)
{
    // 如果已经处理过该文件，就不要再处理，避免循环引用
    if (find(sources.begin(), sources.end(), script_name) != sources.end()) {
        cout << script_name << "is already in list." << endl;
        return;
    }
    

    fs::path obj_name = build_dir;
    obj_name /= script_name.stem();
    obj_name += ".o";
    // 根据时间戳决定是否编译该.cpp文件(生成早的文件数值小)
    if (!exists(obj_name) || last_write_time(script_name) >= last_write_time(obj_name)) {
        cout << "generate " << obj_name << endl;
        //cout << "source time: " << last_write_time(script_name) << " obj time: " << last_write_time(obj_name) << endl;
        // 构造编译器命令行
        string cmd_line;
        cmd_line = "g++ -std\\=c++11 -fmax-errors=1 -c -o "; // -fmax-errors=1让编译器在发现一个错误时就停下来
        cmd_line += obj_name.native();
        cmd_line += " ";
        cmd_line += script_name.native();

        int gcc_status;
        gcc_status = system(cmd_line.c_str());

    }


    cout << "collect sources referenced by " << script_name << endl;

    sources.push_back(script_name);

    ifstream in(script_name.native());
    // if (!in) {
    //     cerr << "no file\n";
    // }

    string line;
    regex pat{R"(using\s+(\w+\.cpp))"};
    while (getline(in,line)) {
        smatch matches;
        if (regex_search(line, matches, pat)) {
            cout << "found " << matches[1] << " in " << script_name << endl;
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
            }
        }
    }

}


int main(int argc, char* argv[])
{
    if (argc != 2) {
        cout << "usage: cpps something.cpp" << endl;
        return 0;
    }

    fs::path script_name(argv[1]);

    if (!exists(script_name)) {
        cout << "No such file.\n";
        return -1;
    }

    if (script_name.extension() != ".cpp") {
        cout << script_name << " should have a .cpp suffix." << endl;
        return -1;
    }

    // 确保~/.cpps/cache的存在
    fs::path home_dir(getpwuid(getuid())->pw_dir);
    fs::path cache_dir_name(".cpps/cache");
    fs::path cache_dir = home_dir;
    cache_dir /= cache_dir_name;
    create_directories(cache_dir);

    // 在cache中创建build文件夹
    fs::path script_dir = canonical(script_name).parent_path().relative_path();
    build_dir = cache_dir;
    build_dir /= script_dir;
    create_directories(build_dir);

    // 确定可执行文件的名字
    fs::path exe_name = build_dir;
    exe_name /= script_name.stem();


    // 确定所有.cpp文件的名字
    vector<fs::path> sources; // 绝对路径
    collect_sources_referenced_by(canonical(script_name), sources);


    PFileEntity exe {new FileEntity(exe_name.filename().string(), exe_name)};
    exe->addAction(PAction(new Obj2ExeAction));
    PPhonyEntity update_dependency{new PhonyEntity("update dependency graph")}; 

    // for src_path in srcListOneByOne:
    for (auto src_path : sources) {

        // 根据.cpp文件的名字，确定.o文件的名字
        fs::path obj_path = build_dir;
        obj_path /= src_path.stem();
        obj_path += ".o";

        // 
        PFileEntity obj{new FileEntity(obj_path.filename().string(), obj_path)};
        obj->addAction(PAction(new Cpp2ObjAction));

        // 可执行文件依赖.o文件
        exe->addPrerequisite(obj);

        // .o文件依赖.cpp文件
        PFileEntity src{new FileEntity(src_path.filename().string(), src_path)};
        obj->addPrerequisite(src);

        // 根据.cpp文件的名字，确定.dep文件的名字
        fs::path dep_path = build_dir;
        dep_path /= src_path.stem();
        dep_path += ".d";

        if (exists(dep_path)) {
            PPhonyEntity obj_update {new PhonyEntity("update for " + obj_path.string())};
            obj_update->addAction(PUpdateDependencyGraphAction(new UpdateDependencyGraphAction(obj)));
            update_dependency->addPrerequisite(obj_update);

            PFileEntity dep {new FileEntity(dep_path.filename().string(), dep_path)};
            obj_update->addPrerequisite(dep);
        }


    } // for
    
    exe->update();



    // 构造编译器命令行
    string cmd_line;
    cmd_line = "g++ -std\\=c++11 -fmax-errors=1"; // -fmax-errors=1让编译器在发现一个错误时就停下来
    for (auto f : sources) {
        cmd_line += " ";
        cmd_line += f.native();
    }
    cmd_line += " -o ";
    cmd_line += exe_name.native();

    cout << "build command: " << cmd_line << endl;

    // 检查编译是否有错无
    int gcc_status;
    gcc_status = system(cmd_line.c_str());
    if (gcc_status)
        return -1;

    // 运行得到的可执行文件
    system(exe_name.c_str());

    return 0;
}
