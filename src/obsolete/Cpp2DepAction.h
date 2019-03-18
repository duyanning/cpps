#ifndef CPP2DEPACTION_H
#define CPP2DEPACTION_H

#include "Action.h"

class Cpp2DepAction : public Action {
public:
    bool execute(EntityPtr target, 
                 vector<EntityPtr>& allPre, 
                 vector<EntityPtr>& changedPre,
                 vector<EntityPtr>& failedPre) override;
};

using Cpp2DepActionPtr = shared_ptr<Cpp2DepAction>;
Cpp2DepActionPtr makeCpp2DepAction();

#endif // CPP2DEPACTION_H
