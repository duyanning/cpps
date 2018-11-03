#ifndef OBJ2EXEACTION_H
#define OBJ2EXEACTION_H

#include "Action.h"

class Obj2ExeAction : public Action {
public:
    Obj2ExeAction(string other_options);
    bool execute(EntityPtr target, 
                 vector<EntityPtr>& allPre, 
                 vector<EntityPtr>& changedPre,
                 vector<EntityPtr>& failedPre
        ) override;
private:
    string m_other_options;
};

using Obj2ExeActionPtr = shared_ptr<Obj2ExeAction>;

Obj2ExeActionPtr makeObj2ExeAction(string other_options);

#endif // OBJ2EXEACTION_H
