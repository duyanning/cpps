#ifndef FILEENTITY_H
#define FILEENTITY_H

#include "DependencyGraphEntity.h"
#include "FileSig.h"

class FileEntity : public DependencyGraphEntity {
public:
    FileEntity(fs::path path);
    string name() override;
    void update() override;
    time_t timestamp() override;
    time_t actualFileTimestamp();
    fs::path path();
    FileSig sig();
private:
    fs::path m_path;
};

using FileEntityPtr = shared_ptr<FileEntity>;

FileEntityPtr makeFileEntity(fs::path path);

#endif // FILEENTITY_H
