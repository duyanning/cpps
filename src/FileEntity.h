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
    // bool loadSig(FileSig& sig);  // 从.sig文件中加载sig。返回值false表示.sig文件并不存在
    // void saveSig(const FileSig& sig); // 将sig保存到.sig文件中
    virtual bool needExecuteActions(const DepInfo& info);
    virtual bool isFile() override {  return true;  }

    //void generate_birth_cert(fs::path dep_path);
    void generate_birth_cert();
private:
    void generate_birth_cert(const vector<fs::path>& pre_file_paths);
    //void get_src_sigs_from_birthcert(vector<FileSig>& sig_vector);
    fs::path m_path;
};

using FileEntityPtr = shared_ptr<FileEntity>;

inline
FileEntityPtr makeFileEntity(fs::path path)
{
    return FileEntityPtr(new FileEntity(path));
}

#endif // FILEENTITY_H
