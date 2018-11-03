#ifndef DEPENDENCYGRAPHENTITY_H
#define DEPENDENCYGRAPHENTITY_H

class DependencyGraphEntity;

using EntityPtr = shared_ptr<DependencyGraphEntity>;

#include "Action.h"

class DependencyGraphEntity : public enable_shared_from_this<DependencyGraphEntity> {
public:
    DependencyGraphEntity();
    virtual ~DependencyGraphEntity() = 0;
    void addAction(ActionPtr action);
    void addPrerequisite(EntityPtr p);
    virtual bool update() = 0;
    void updatePrerequisites(vector<EntityPtr>& changed, vector<EntityPtr>& failed);
    virtual time_t timestamp() = 0; // 相当于文件的生日，数字越小，文件越老。
    virtual string name() = 0;
    //void executeActions(EntityPtr target, vector<EntityPtr>&  allPre, vector<EntityPtr>& changedPre);
    bool executeActions(EntityPtr target,
                        vector<EntityPtr>& allPre,
                        vector<EntityPtr>& changedPre,
                        vector<EntityPtr>& failedPre);
    void show(ostream& os, int level = 0, string indent = "  ");
protected:
    vector<EntityPtr> prerequisiteList;
    vector<ActionPtr> actions;
private:
};



#endif // DEPENDENCYGRAPHENTITY_H
