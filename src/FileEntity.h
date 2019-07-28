#ifndef FILEENTITY_H
#define FILEENTITY_H

#include "Entity.h"
#include "FileSig.h"
#include "DepInfo.h"

class FileEntity;
using FileEntityPtr = shared_ptr<FileEntity>;

class FileEntity : public Entity {
public:
    //FileEntity(fs::path path);
    string name() override;
    bool update() override;
    //time_t timestamp() override;
    time_t actualFileTimestamp();
    fs::path path();
    FileSig sig();
    virtual bool needExecuteActions(const DepInfo& info);
    virtual bool isFile() override {  return true;  }

    void generate_birth_cert(fs::path dep_path);
    void generate_birth_cert();
private:
    void generate_birth_cert(const vector<fs::path>& pre_file_paths);
    //void get_src_sigs_from_birthcert(vector<FileSig>& sig_vector);
    fs::path m_path;

    friend FileEntityPtr makeFileEntity(fs::path path);
private:
    FileEntity(fs::path path);
};


FileEntityPtr makeFileEntity(fs::path path);

#endif // FILEENTITY_H
