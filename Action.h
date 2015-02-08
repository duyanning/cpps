#ifndef ACTION_H
#define ACTION_H

class Action;
typedef shared_ptr<Action> PAction;

#include "DependencyGraphEntity.h"

class Action : public enable_shared_from_this<Action> {
public:
    virtual void execute(PDependencyGraphEntity target, vector<PDependencyGraphEntity>&  allPre, vector<PDependencyGraphEntity>& changedPre) = 0;
};



#endif // ACTION_H
