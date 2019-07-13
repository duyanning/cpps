#include "config.h"
#include "VcObj2ExeAction.h"
#include "FileEntity.h"
#include "Loggers.h"
#include "global.h"

VcObj2ExeAction::VcObj2ExeAction(string other_options)
    :
    m_other_options(other_options)
{
}

VcObj2ExeActionPtr makeVcObj2ExeAction(string other_options)
{
    return VcObj2ExeActionPtr(new VcObj2ExeAction(other_options));
}

bool VcObj2ExeAction::execute(const DepInfo& info)
{
    // 如果exe依赖的某个.o更新失败，那就无法生成exe
    if (!info.failed.empty()) {
        //cout << "VcObj2ExeAction failed" << endl;
        return false;
    }
    
    // 构造命令行
    FileEntityPtr exe = static_pointer_cast<FileEntity>(info.target);
    fs::path exe_path = exe->path();

    fs::path dep_path = exe_path;
    dep_path += ".d";

    // fs::path birthcert_path = exe_path;
    // birthcert_path += ".birthcert";

    string cmd = vc_link_cmd;
    //cmd += " -o";
	cmd += " /Fe:";

    cmd += " ";

    cmd += exe_path.string();

    for (auto p : info.all) {
        FileEntityPtr f = static_pointer_cast<FileEntity>(p);
        cmd = cmd + " " + f->path().string();
    }

    cmd += " ";
    cmd += m_other_options;

    // -l 选项的位置很重要，必须放在使用它的.o文件之后
    cmd += extra_link_flags;

    MINILOG(build_exe_summay_logger, "linking " << exe_path.filename().string());
    MINILOG(build_exe_detail_logger, cmd);

    // 确保目录存在
    create_directories(exe_path.parent_path());

    // 链接
    int gcc_status;
    gcc_status = system(cmd.c_str());
    if (gcc_status)
        return false;

    exe->generate_birth_cert();

    return true;
}
