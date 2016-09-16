#include "std.h"
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

void MustExistFileEntity::update()
{
    if (!exists(this->path())) {
        string errMsg = name()  + ": cannot make `" + this->name() + "'";
        cout << errMsg << endl; 
        throw 1;
    }
}
