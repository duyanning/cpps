#include "std.h"
#include "Obj2ExeAction.h"
#include "FileEntity.h"
#include "Loggers.h"

Obj2ExeAction::Obj2ExeAction(string other_options)
    :
    m_other_options(other_options)
{
}

Obj2ExeActionPtr makeObj2ExeAction(string other_options)
{
    return Obj2ExeActionPtr(new Obj2ExeAction(other_options));
}

void Obj2ExeAction::execute(EntityPtr target, vector<EntityPtr>&  allPre, vector<EntityPtr>& changedPre)
{
    // 构造命令行
    FileEntityPtr exe = static_pointer_cast<FileEntity>(target);
    fs::path exe_path = exe->path();

    string cmd = "g++ -std=c++11 -fmax-errors=1 -o ";

    cmd += exe_path.string();

    for (auto p : allPre) {
        FileEntityPtr f = static_pointer_cast<FileEntity>(p);
        cmd = cmd + " " + f->path().string();
    }

    cmd += " ";
    cmd += m_other_options;

    MINILOG(build_exe_summay_logger, "linking " << exe_path.filename().string());
    MINILOG(build_exe_detail_logger, cmd);
    // 链接
    int gcc_status;
    gcc_status = system(cmd.c_str());
    if (gcc_status)
        throw gcc_status;
}
