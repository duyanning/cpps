#include "std.h"
#include "Obj2ExeAction.h"
#include "FileEntity.h"

Obj2ExeActionPtr makeObj2ExeAction()
{
    return Obj2ExeActionPtr(new Obj2ExeAction);
}

void Obj2ExeAction::execute(PDependencyGraphEntity target,
                            vector<PDependencyGraphEntity>&  allPre, vector<PDependencyGraphEntity>& changedPre)
{
    // 构造命令行
    FileEntityPtr exe = static_pointer_cast<FileEntity>(target);
    fs::path exe_path = exe->path();

    string cmd = "g++ -o ";

    cmd += exe_path.string();

    for (auto p : allPre) {
        FileEntityPtr f = static_pointer_cast<FileEntity>(p);
        cmd = cmd + " " + f->path().string();
    }

    // 链接
    int gcc_status;
    gcc_status = system(cmd.c_str());
    if (gcc_status)
        throw gcc_status;
}
