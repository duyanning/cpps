#ifndef FILEENTITY_H
#define FILEENTITY_H

#include "DependencyGraphEntity.h"

class FileEntity : public DependencyGraphEntity {
public:
    FileEntity(string name, fs::path path);
    void update();
    time_t timestamp();
    time_t actualFileTimestamp();
    fs::path path();
private:
    fs::path m_path;
};

typedef shared_ptr<FileEntity> FileEntityPtr;

FileEntityPtr makeFileEntity(string name, fs::path path);

#endif // FILEENTITY_H
