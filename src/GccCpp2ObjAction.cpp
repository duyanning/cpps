#include "config.h"
#include "GccCpp2ObjAction.h"
//#include "VulnerableFileEntity.h"
#include "FileEntity.h"
#include "Loggers.h"
#include "helpers.h"
#include "global.h"

bool GccCpp2ObjAction::execute(const DepInfo& info)
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

    fs::path dep_path = obj_path;
    dep_path += ".d";

    // fs::path birthcert_path = obj_path;
    // birthcert_path += ".birthcert";

    string cmd = compile_cpp_cmd;

    cmd += compiler_specific_extra_compile_flags[cpp_path.string()];
    
    cmd += " ";
    cmd += compile_cmd_include_dirs;

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
    // 函数system给出的就是gcc的返回值
    // 在shell下执行gcc后，可用echo $?来查看该返回值
    // 有编译错误时，该返回值为1，无错误时为0
    // 即便有警告，仍旧是0。
    //cout << "cpp2obj:" << gcc_status << endl;
    if (gcc_status)             // 非0就是代表失败了
        return false;

    // 产生出生证明文件（gcc编译时，如果遇到#include的头文件不存在，就算fatal error，也不会生成.d文件）
	// 从本次运行新生成的.d生成.birthcert，可以保证二者一致。依赖图反映的是老.d
    obj->generate_birth_cert(dep_path);
    //obj->generate_birth_cert();

    return true;

}
