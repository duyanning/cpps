#include "config.h"
#include "Cpp2ObjAction.h"      // using Action.cpp
#include "VulnerableFileEntity.h"
#include "Loggers.h"
#include "helpers.h"
#include "global.h"

Cpp2ObjActionPtr makeCpp2ObjAction()
{
    return Cpp2ObjActionPtr(new Cpp2ObjAction);
}

bool Cpp2ObjAction::execute(DepInfo& info)
{
    if (!info.failed.empty()) {
        //cout << "!failedPre.empty()";
        for (auto pre : info.failed) {
            FileEntityPtr fe = static_pointer_cast<FileEntity>(pre);
            if (is_a_cpp_src(fe->path())) return false;
        }
    }
    // 构造命令行
    FileEntityPtr cpp = static_pointer_cast<FileEntity>(info.all[0]);
    fs::path cpp_path = cpp->path();

    VulnerableFileEntityPtr obj = static_pointer_cast<VulnerableFileEntity>(info.target);
    fs::path obj_path = obj->path();

    fs::path dep_path = obj_path;
    dep_path += ".d";

    fs::path birthcert_path = obj_path;
    birthcert_path += ".birthcert";

    string cmd = gcc_compile_cpp_cmd;
    cmd += " -o";

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

    // 产生出生证明文件（gcc编译时，如果遇到#include的头文件不存在，就算fatal error，也不会生成.d文件）
    obj->generate_birth_cert(dep_path);

    return true;

}
