#ifndef UPDATEDEPENDENCYGRAPHACTION_H
#define UPDATEDEPENDENCYGRAPHACTION_H

#include "Action.h"
#include "FileEntity.h"

class UpdateDependencyGraphAction : public Action {
public:
    UpdateDependencyGraphAction(PFileEntity obj);
    void execute(PDependencyGraphEntity target, vector<PDependencyGraphEntity>&  allPre, vector<PDependencyGraphEntity>& changedPre);
private:
    PFileEntity m_obj;
};

typedef shared_ptr<UpdateDependencyGraphAction> PUpdateDependencyGraphAction;

#endif // UPDATEDEPENDENCYGRAPHACTION_H
