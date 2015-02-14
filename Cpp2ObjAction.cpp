#include "std.h"
#include "Cpp2ObjAction.h"
#include "FileEntity.h"
#include "Loggers.h"
#include "helpers.h"

Cpp2ObjActionPtr makeCpp2ObjAction()
{
    return Cpp2ObjActionPtr(new Cpp2ObjAction);
}

void Cpp2ObjAction::execute(EntityPtr target, vector<EntityPtr>&  allPre, vector<EntityPtr>& changedPre)
{
    // 构造命令行
    FileEntityPtr cpp = static_pointer_cast<FileEntity>(allPre[0]);
    fs::path cpp_path = cpp->path();

    FileEntityPtr obj = static_pointer_cast<FileEntity>(target);
    fs::path obj_path = obj->path();

    fs::path dep_path = obj_path;
    dep_path += ".d";

    fs::path birthcert_path = obj_path;
    birthcert_path += ".birthcert";

    string cmd = "g++ -std=c++11 -fmax-errors=1 -Wall -c -o";

    cmd += " ";
    cmd += obj_path.string();

    cmd += " ";
    cmd += cpp_path.string();

    cmd += " -fpch-deps -MMD -MF " + dep_path.string();

    MINILOG(build_exe_summay_logger, "compiling " << cpp_path.filename().string());
    MINILOG(build_exe_detail_logger, cmd);

    // 确保目录存在
    create_directories(obj_path.parent_path());

    // 产生.o文件和.d文件
    int gcc_status;
    gcc_status = system(cmd.c_str());
    if (gcc_status)
        throw gcc_status;

    // 产生出生证明文件
    ofstream ofs(birthcert_path.string());
    ofs << cpp->sig().timestamp << "\n" << cpp->sig().size << endl;
}
