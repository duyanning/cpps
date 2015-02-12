#ifndef DEPENDENCYGRAPHENTITY_H
#define DEPENDENCYGRAPHENTITY_H

class DependencyGraphEntity;
typedef shared_ptr<DependencyGraphEntity> EntityPtr;

#include "Action.h"

class DependencyGraphEntity : public enable_shared_from_this<DependencyGraphEntity> {
public:
    DependencyGraphEntity();
    void addAction(ActionPtr action);
    void addPrerequisite(EntityPtr p);
    virtual void update() = 0;
    void updatePrerequisites(vector<EntityPtr>& changed);
    virtual time_t timestamp() = 0;
    virtual string name() = 0;
    void executeActions(EntityPtr target, vector<EntityPtr>&  allPre, vector<EntityPtr>& changedPre);
    void show(ostream& os, int level = 0, string indent = "  ");
protected:
    vector<EntityPtr> prerequisiteList;
    vector<ActionPtr> actions;
private:
};



#endif // DEPENDENCYGRAPHENTITY_H
