#include "config.h"
#include "MustExistFileEntity.h"

MustExistFileEntity::MustExistFileEntity(fs::path path)
:
    FileEntity(path)
{
}

MustExistFileEntityPtr makeMustExistFileEntity(fs::path path)
{
    return MustExistFileEntityPtr(new MustExistFileEntity(path));
}

bool MustExistFileEntity::update()
{
    if (!exists(this->path())) {
        // string errMsg = name()  + ": cannot make `" + this->name() + "'";
        // cout << errMsg << endl; 
        // throw 1;
        return false;
    }
    return true;
}
