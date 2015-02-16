#ifndef VULNERABLEFILEENTITY_H
#define VULNERABLEFILEENTITY_H

#include "FileEntity.h"

// .o文件、.gch文件，都是从用户提供的文件（.cpp、.h）直接生成，而用户那
// 边啥事情都可能发生，所以在生成这些文件时，除了根据时间戳的先后决定是
// 否生成外，还要看出生证明上写的源文件是不是现在这个源文件
class VulnerableFileEntity : public FileEntity {
public:
    VulnerableFileEntity(fs::path path);
    void update() override;
    void src_sig_vector(vector<FileSig>& sig_vector);
    void write_birth_cert(fs::path dep_path);
};

using VulnerableFileEntityPtr = shared_ptr<VulnerableFileEntity>;
VulnerableFileEntityPtr makeVulnerableFileEntity(fs::path path);

#endif // VULNERABLEFILEENTITY_H
