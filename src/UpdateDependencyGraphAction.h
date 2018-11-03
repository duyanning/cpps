#ifndef UPDATEDEPENDENCYGRAPHACTION_H
#define UPDATEDEPENDENCYGRAPHACTION_H

#include "Action.h"
#include "FileEntity.h"

class UpdateDependencyGraphAction : public Action {
public:
    UpdateDependencyGraphAction(FileEntityPtr obj);
    bool execute(EntityPtr target,
                 vector<EntityPtr>& allPre,
                 vector<EntityPtr>& changedPre,
                 vector<EntityPtr>& failedPre
        ) override;
private:
    FileEntityPtr m_obj;
};

using UpdateDependencyGraphActionPtr = shared_ptr<UpdateDependencyGraphAction>;

UpdateDependencyGraphActionPtr makeUpdateDependencyGraphAction(FileEntityPtr obj);

#endif // UPDATEDEPENDENCYGRAPHACTION_H
