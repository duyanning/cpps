#ifndef ACTION_H
#define ACTION_H


#include "Entity.h"
#include "DepInfo.h"

// class Action;
// using ActionPtr = shared_ptr<Action>;


class Action : public enable_shared_from_this<Action> {
public:
    //virtual void execute(EntityPtr target, vector<EntityPtr>&  allPre, vector<EntityPtr>& changedPre) = 0;
    virtual bool execute(DepInfo& info) = 0;
    // virtual bool execute(EntityPtr target, vector<EntityPtr>& allPre,
    //                      vector<EntityPtr>& changedPre,
    //                      vector<EntityPtr>& failedPre
    //     ) = 0;
    virtual ~Action() = 0;
};



#endif // ACTION_H
