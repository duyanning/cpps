#ifndef FILEENTITY_H
#define FILEENTITY_H

#include "DependencyGraphEntity.h"

struct FileSig {
    fs::path path;
    time_t timestamp {0};            // 文件的日期
    uintmax_t size {0};             // 文件的大小
};

inline
bool operator==(FileSig a, FileSig b)
{
    if (a.timestamp == b.timestamp && a.size == b.size)
        return true;
    return false;
}

inline
bool operator!=(FileSig a, FileSig b)
{
    return !(a == b);
}

inline
ostream& operator<<(ostream& os, const FileSig& sig)
{
    return os << sig.path << "\n" << sig.timestamp << "\n" << sig.size << "\n";
}

inline
istream& operator>>(istream& is, FileSig& sig)
{
    return is >> sig.path >> sig.timestamp >> sig.size;
}

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
