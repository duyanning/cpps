#ifndef ACTION_H
#define ACTION_H

class Action;
typedef shared_ptr<Action> ActionPtr;

#include "DependencyGraphEntity.h"

class Action : public enable_shared_from_this<Action> {
public:
    virtual void execute(EntityPtr target, vector<EntityPtr>&  allPre, vector<EntityPtr>& changedPre) = 0;
};



#endif // ACTION_H
