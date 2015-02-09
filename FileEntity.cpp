#include "std.h"
#include "FileEntity.h"

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
        }
        if (this->timestamp() < p->timestamp()) {
            needExecute = true;
            break;
        }
    }

    // execute action
    if (needExecute)
        executeActions(shared_from_this(), prerequisiteList, changed);
}


time_t FileEntity::actualFileTimestamp()
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

time_t FileEntity::timestamp()
{
    return actualFileTimestamp();
}
