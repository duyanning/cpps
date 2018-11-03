#ifndef H2GCHACTION_H
#define H2GCHACTION_H

#include "Action.h"

class H2GchAction : public Action {
public:
    bool execute(EntityPtr target, 
                 vector<EntityPtr>& allPre, 
                 vector<EntityPtr>& changedPre,
                 vector<EntityPtr>& failedPre
        ) override;
};

using H2GchActionPtr = shared_ptr<H2GchAction>;

H2GchActionPtr makeH2GchAction();



#endif // H2GCHACTION_H
