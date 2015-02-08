#ifndef OBJ2EXEACTION_H
#define OBJ2EXEACTION_H

#include "Action.h"

class Obj2ExeAction : public Action {
public:
    void execute(PDependencyGraphEntity target, vector<PDependencyGraphEntity>&  allPre, vector<PDependencyGraphEntity>& changedPre);

};

typedef shared_ptr<Obj2ExeAction> Obj2ExeActionPtr;
Obj2ExeActionPtr makeObj2ExeAction();

#endif // OBJ2EXEACTION_H
