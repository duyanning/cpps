#include "config.h"
#include "FileEntity.h" // using Entity.cpp

FileEntity::FileEntity(fs::path path)
:
    m_path(path)
{
}

string FileEntity::name()
{
    return m_path.string();
}


fs::path FileEntity::path()
{
    return m_path;
}

bool FileEntity::update()
{
    // 如果一个文件节点是个叶子节点
    if (prerequisiteList.empty()) {
        return exists(m_path);  // 存在就好，不存在那就真没办法了
    }

    // 不是叶子节点的话，先把所有的下级节点更新一下
    DepInfo info;
    info.target = shared_from_this();
    info.all = prerequisiteList;

    updatePrerequisites(info);


    // 然后再判断是否要执行本节点关联的动作
    if (needExecuteActions(info))
        return executeActions(info);

    return true;
}

bool FileEntity::needExecuteActions(const DepInfo& info)
{
    return !info.changed.empty();
}

// 若文件不存在，就认为它的生日为0
// 若它作为目标文件，生日为0意味着这是一个最古老的文件，它比任何依赖文件都要老，所以必须重新生成
// 若它作为依赖文件，生日0意味着文件不存在，没法用它生成目标文件
// todo：如果一个文件不存在，让这个函数抛出异常。
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
