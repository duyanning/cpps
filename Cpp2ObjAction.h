#ifndef CPP2OBJACTION_H
#define CPP2OBJACTION_H

#include "Action.h"

class Cpp2ObjAction : public Action {
public:
    void execute(PDependencyGraphEntity target, vector<PDependencyGraphEntity>&  allPre, vector<PDependencyGraphEntity>& changedPre);
};

typedef shared_ptr<Cpp2ObjAction> Cpp2ObjActionPtr;

#endif // CPP2OBJACTION_H
