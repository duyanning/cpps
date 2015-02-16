#include "std.h"
#include "FileEntity.h" // using DependencyGraphEntity.cpp

FileEntity::FileEntity(fs::path path)
:
    m_path(path)
{
}

string FileEntity::name()
{
    return m_path.string();
}


FileEntityPtr makeFileEntity(fs::path path)
{
    return FileEntityPtr(new FileEntity(path));
}

fs::path FileEntity::path()
{
    return m_path;
}

void FileEntity::update()
{
    // make all prerequisites
    vector<EntityPtr> changed;
    updatePrerequisites(changed);

    // check to see if execution is needed
    bool needExecute = false;
    for (auto p : prerequisiteList) {
        if (p->timestamp() == 0) {
            string errMsg = name()  + ": cannot make `" + p->name() + "'";
            cout << errMsg << endl;
            throw 1;
        }

        if (this->timestamp() < p->timestamp()) { // todo: if two phonyentities compare? <=
            needExecute = true;
            break;
        }
    }

    // execute action
    if (needExecute)
        executeActions(shared_from_this(), prerequisiteList, changed);
}

// 若文件不存在，就认为它的生日为0
// 若它作为目标文件，生日为0意味着这是一个最古老的文件，它比任何依赖文件都要老，所以必须重新生成
// 若它作为依赖文件，生日0意味着文件不存在，没法用它生成目标文件
time_t FileEntity::timestamp()
{
    time_t timestamp;
    if (!exists(m_path))
        timestamp = 0;
    else {
        // timestamp is the modification time of file
        timestamp = last_write_time(m_path);
    }
    return timestamp;
}

FileSig FileEntity::sig()
{
    FileSig sig;
    sig.path = m_path;
    sig.timestamp = last_write_time(m_path);
    sig.size = file_size(m_path);
    return sig;
}

