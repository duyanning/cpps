#include "config.h"
#include "Cpp2DepAction.h"      // using Action.cpp
#include "VulnerableFileEntity.h"
#include "Loggers.h"
#include "helpers.h"
#include "global.h"

Cpp2DepActionPtr makeCpp2DepAction()
{
    return Cpp2DepActionPtr(new Cpp2DepAction);
}

bool Cpp2DepAction::execute(EntityPtr target, 
                            vector<EntityPtr>& allPre, 
                            vector<EntityPtr>& changedPre,
                            vector<EntityPtr>& failedPre)
{
    // 构造命令行
    FileEntityPtr cpp = static_pointer_cast<FileEntity>(allPre[0]);
    fs::path cpp_path = cpp->path();

    //VulnerableFileEntityPtr obj = static_pointer_cast<VulnerableFileEntity>(target);
    FileEntityPtr dep = static_pointer_cast<FileEntity>(target);
    fs::path dep_path = dep->path();

    fs::path obj_path = dep_path.stem(); // 去掉.d得到.o
    //dep_path += ".d";

    // fs::path birthcert_path = obj_path;
    // birthcert_path += ".birthcert";

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
    // obj->generate_birth_cert(dep_path);

    return true;
}
