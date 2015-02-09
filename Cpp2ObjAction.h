#ifndef CPP2OBJACTION_H
#define CPP2OBJACTION_H

#include "Action.h"

class Cpp2ObjAction : public Action {
public:
    void execute(EntityPtr target, vector<EntityPtr>&  allPre, vector<EntityPtr>& changedPre);
};

typedef shared_ptr<Cpp2ObjAction> Cpp2ObjActionPtr;

#endif // CPP2OBJACTION_H
