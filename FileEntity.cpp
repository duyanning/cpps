#include "std.h"
#include "FileEntity.h"

FileEntity::FileEntity(string name, fs::path path)
:
    DependencyGraphEntity(name),
    m_path(path)
{
}

FileEntityPtr makeFileEntity(string name, fs::path path)
{
    return FileEntityPtr(new FileEntity(name, path));
}

fs::path FileEntity::path()
{
    return m_path;
}

void FileEntity::update()
{
    // make all prerequisites
    vector<PDependencyGraphEntity> changed;
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
