#include "std.h"
#include "DependencyGraphEntity.h"

DependencyGraphEntity::DependencyGraphEntity()
{
}

void DependencyGraphEntity::addAction(shared_ptr<Action> action)
{
    actions.push_back(action);
}

void DependencyGraphEntity::addPrerequisite(EntityPtr p)
{
    prerequisiteList.push_back(p);
}


void DependencyGraphEntity::updatePrerequisites(vector<EntityPtr>& changed)
{
    for (auto p : prerequisiteList) {
        time_t oldStamp = p->timestamp();
        p->update();
        if (p->timestamp() > oldStamp)
            changed.push_back(p);
    }
            
}

void DependencyGraphEntity::executeActions(EntityPtr target, vector<EntityPtr>&  allPre, vector<EntityPtr>& changedPre)
{
    for (auto a : actions)
        a->execute(target, allPre, changedPre);
}

void DependencyGraphEntity::show(ostream& os, int level, string indent)
{
    for (int i = 0; i < level; i++)
        os << indent;
    os << name() << endl;

    for (auto p : prerequisiteList)
        p->show(os, level + 1);
}
