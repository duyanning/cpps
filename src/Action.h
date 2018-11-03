#ifndef ACTION_H
#define ACTION_H

class Action;
using ActionPtr = shared_ptr<Action>;

#include "DependencyGraphEntity.h"

class Action : public enable_shared_from_this<Action> {
public:
    //virtual void execute(EntityPtr target, vector<EntityPtr>&  allPre, vector<EntityPtr>& changedPre) = 0;
    virtual bool execute(EntityPtr target, vector<EntityPtr>& allPre,
                         vector<EntityPtr>& changedPre,
                         vector<EntityPtr>& failedPre
        ) = 0;
    virtual ~Action() = 0;
};



#endif // ACTION_H
