#ifndef MUSTEXISTFILEENTITY_H
#define MUSTEXISTFILEENTITY_H

#include "FileEntity.h"

class MustExistFileEntity : public FileEntity {
public:
    MustExistFileEntity(fs::path path);
    void update() override;
    FileSig src_sig();
};

using MustExistFileEntityPtr = shared_ptr<MustExistFileEntity>;
MustExistFileEntityPtr makeMustExistFileEntity(fs::path path);

#endif // MUSTEXISTFILEENTITY_H
