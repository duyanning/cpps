#ifndef FILEENTITY_H
#define FILEENTITY_H

#include "DependencyGraphEntity.h"

class FileEntity : public DependencyGraphEntity {
public:
    FileEntity(string name, fs::path path);
    void update();
    time_t timestamp();
    time_t actualFileTimestamp();
private:
    fs::path m_path;
};

typedef shared_ptr<FileEntity> PFileEntity;

#endif // FILEENTITY_H
