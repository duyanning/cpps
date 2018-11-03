#ifndef FILEENTITY_H
#define FILEENTITY_H

#include "Entity.h"
#include "FileSig.h"
#include "DepInfo.h"

class FileEntity : public Entity {
public:
    FileEntity(fs::path path);
    string name() override;
    bool update() override;
    time_t timestamp() override;
    time_t actualFileTimestamp();
    fs::path path();
    FileSig sig();
    virtual bool needExecuteActions(DepInfo& info);
private:
    fs::path m_path;
};

using FileEntityPtr = shared_ptr<FileEntity>;

FileEntityPtr makeFileEntity(fs::path path);

#endif // FILEENTITY_H
