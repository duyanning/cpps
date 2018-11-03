#include "config.h"
#include "H2GchAction.h"
#include "VulnerableFileEntity.h"
#include "helpers.h"
#include "Loggers.h"
#include "global.h"

H2GchActionPtr makeH2GchAction()
{
    return H2GchActionPtr(new H2GchAction);
}

bool H2GchAction::execute(EntityPtr target, 
                          vector<EntityPtr>& allPre, 
                          vector<EntityPtr>& changedPre,
                          vector<EntityPtr>& failedPre
    )
{
    // 构造命令行
    FileEntityPtr h = static_pointer_cast<FileEntity>(allPre[0]);
    fs::path h_path = h->path();

    VulnerableFileEntityPtr gch = static_pointer_cast<VulnerableFileEntity>(target);
    fs::path gch_path = gch->path();

    fs::path dep_path = shadow(gch_path);
    dep_path += ".d";

    fs::path birthcert_path = gch_path;
    birthcert_path += ".birthcert";

    // we should remove old .d before generate new one
    // otherwise, we will get a warning message
    if (exists(dep_path))
        remove(dep_path);

    string cmd = gcc_compile_h_cmd;
    cmd += " -o";

    cmd += " ";
    cmd += gch_path.string();

    cmd += " ";
    cmd += h_path.string();

    cmd += " -fpch-deps -MMD -MF " + dep_path.string();

    MINILOG(build_gch_summay_logger, "precompiling " << h_path.filename().string());
    MINILOG(build_gch_detail_logger, cmd);

    // 因为预编译头文件要生成在.h文件所在目录，你不一定能有权限
    if (!can_write_in(gch_path.parent_path())) {
        MINILOG(perm_logger, "permissin prevent from generating " << gch_path);
        return true;
    }

    // 确保目录存在
    create_directories(dep_path.parent_path());

    // 产生.gch和.d文件
    int gcc_status;
    gcc_status = system(cmd.c_str());
    if (gcc_status)
        throw gcc_status;

    // 产生出生证明文件（gcc编译时，如果遇到#include的头文件不存在，就算fatal error，也不会生成.d文件）
    gch->generate_birth_cert(dep_path);

    return true;
}

