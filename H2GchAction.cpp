#include "std.h"
#include "H2GchAction.h"
#include "FileEntity.h"
#include "helpers.h"
#include "Loggers.h"

H2GchActionPtr makeH2GchAction()
{
    return H2GchActionPtr(new H2GchAction);
}

void H2GchAction::execute(EntityPtr target, vector<EntityPtr>&  allPre, vector<EntityPtr>& changedPre)
{
    // 构造命令行
    FileEntityPtr h = static_pointer_cast<FileEntity>(allPre[0]);
    fs::path h_path = h->path();

    FileEntityPtr gch = static_pointer_cast<FileEntity>(target);
    fs::path gch_path = gch->path();

    fs::path dep_path = shadow(gch_path);
    dep_path += ".d";

    // we should remove old .d before generate new one
    // otherwise, we will get a warning message
    if (exists(dep_path))
        remove(dep_path);

    string cmd = "g++ -std=c++11 -fmax-errors=1 -Wall -o";

    cmd += " ";
    cmd += gch_path.string();

    cmd += " ";
    cmd += h_path.string();

    cmd += " -fpch-deps -MMD -MF " + dep_path.string();

    MINILOG(build_gch_summay_logger, "precompiling " << h_path.filename().string());
    MINILOG(build_gch_detail_logger, cmd);
    // 产生.gch和.d文件
    int gcc_status;
    gcc_status = system(cmd.c_str());
    if (gcc_status)
        throw gcc_status;
}

