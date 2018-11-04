#include "config.h"
#include "Cpp2ObjAction.h"
#include "VulnerableFileEntity.h"
#include "Loggers.h"
#include "helpers.h"
#include "global.h"

bool Cpp2ObjAction::execute(const DepInfo& info)
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
    //cout << "cpp2obj:" << gcc_status << endl;
    if (gcc_status)
        return false;

    // 产生出生证明文件（gcc编译时，如果遇到#include的头文件不存在，就算fatal error，也不会生成.d文件）
    obj->generate_birth_cert(dep_path);

    return true;

}
