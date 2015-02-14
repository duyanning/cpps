#include "std.h"
#include "VulnerableFileEntity.h"


VulnerableFileEntity::VulnerableFileEntity(fs::path path)
:
    FileEntity(path)
{
}

VulnerableFileEntityPtr makeVulnerableFileEntity(fs::path path)
{
    return VulnerableFileEntityPtr(new VulnerableFileEntity(path));
}

void VulnerableFileEntity::update()
{
    // make all prerequisites
    vector<EntityPtr> changed;
    updatePrerequisites(changed);

    FileEntityPtr src = static_pointer_cast<FileEntity>(prerequisiteList[0]);
    if (src->timestamp() == 0) {
        string errMsg = name()  + ": cannot make `" + src->name() + "'";
        cout << errMsg << endl;
        throw 1;
    }

    // check to see if execution is needed
    bool needExecute = false;
    if (this->timestamp() == 0  // 如果目标文件不存在
        || this->src_sig() != src->sig() // 目标文件就不是由这个源文件产生的
        || this->timestamp() < src->timestamp() // 目标文件比较老
        ) { 
        needExecute = true;
    }
    
    // execute action
    if (needExecute)
        executeActions(shared_from_this(), prerequisiteList, changed);
}

FileSig VulnerableFileEntity::src_sig()
{
    // 从自己的出生证明文件中读取生成自己的源文件的签名

    // 获取自己的出生证明文件的路径（自己的出生证明文件总是跟自己在一起）
    fs::path birthcert_path = path();
    birthcert_path += ".birthcert";

    ifstream ifs(birthcert_path.string());
    FileSig sig;
    ifs >> sig.timestamp >> sig.size;
    return sig;
}

