#include "std.h"
#include "Cpp2ObjAction.h"
#include "FileEntity.h"

void Cpp2ObjAction::execute(PDependencyGraphEntity target,
                            vector<PDependencyGraphEntity>&  allPre, vector<PDependencyGraphEntity>& changedPre)
{
    // 构造命令行
    FileEntityPtr cpp = static_pointer_cast<FileEntity>(allPre[0]);
    fs::path cpp_path = cpp->path();

    FileEntityPtr obj = static_pointer_cast<FileEntity>(target);
    fs::path obj_path = obj->path();

    fs::path dep_path = obj_path.parent_path();
    dep_path += obj_path.stem();
    dep_path += ".d";

    string cmd = "g++ -Wall -c -o";

    cmd += " " + obj_path.string();

    //     for p in allPre:
    //         if p.path().endswith(".cpp"):
    //             cmd += " " + p.path()
    cmd += cpp_path.string();

    cmd += " -fpch-deps -MMD -MF " + dep_path.string();

    // 产生.o文件和.d文件
    int gcc_status;
    gcc_status = system(cmd.c_str());
    if (gcc_status)
        throw gcc_status;

}
