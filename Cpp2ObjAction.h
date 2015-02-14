#ifndef CPP2OBJACTION_H
#define CPP2OBJACTION_H

#include "Action.h"

class Cpp2ObjAction : public Action {
public:
    void execute(EntityPtr target, vector<EntityPtr>&  allPre, vector<EntityPtr>& changedPre) override;
};

using Cpp2ObjActionPtr = shared_ptr<Cpp2ObjAction>;
Cpp2ObjActionPtr makeCpp2ObjAction();

#endif // CPP2OBJACTION_H
