#ifndef UPDATEDEPENDENCYGRAPHACTION_H
#define UPDATEDEPENDENCYGRAPHACTION_H

#include "Action.h"
#include "FileEntity.h"

class UpdateDependencyGraphAction : public Action {
public:
    UpdateDependencyGraphAction(FileEntityPtr obj);
    void execute(EntityPtr target, vector<EntityPtr>&  allPre, vector<EntityPtr>& changedPre);
private:
    FileEntityPtr m_obj;
};

typedef shared_ptr<UpdateDependencyGraphAction> UpdateDependencyGraphActionPtr;

UpdateDependencyGraphActionPtr makeUpdateDependencyGraphAction(FileEntityPtr obj);

#endif // UPDATEDEPENDENCYGRAPHACTION_H
