#ifndef UPDATEGRAPHACTION_H
#define UPDATEGRAPHACTION_H

#include "Action.h"
#include "FileEntity.h"

class UpdateGraphAction : public Action {
public:
    UpdateGraphAction(FileEntityPtr obj);
    bool execute(EntityPtr target,
                 vector<EntityPtr>& allPre,
                 vector<EntityPtr>& changedPre,
                 vector<EntityPtr>& failedPre
        ) override;
private:
    FileEntityPtr m_obj;
};

using UpdateGraphActionPtr = shared_ptr<UpdateGraphAction>;

UpdateGraphActionPtr makeUpdateGraphAction(FileEntityPtr obj);

#endif // UPDATEGRAPHACTION_H
