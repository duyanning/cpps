#include "config.h"
#include "Entity.h"

Entity::Entity()
{
}

Entity::~Entity()
{
}

void Entity::addAction(shared_ptr<Action> action)
{
    actions.push_back(action);
}

void Entity::addPrerequisite(EntityPtr p)
{
    prerequisiteList.push_back(p);
}


void Entity::updatePrerequisites(vector<EntityPtr>& changed, vector<EntityPtr>& failed)
{
    for (auto p : prerequisiteList) {
        time_t oldStamp = p->timestamp();
        bool success = p->update();
        if (success) {
            if (p->timestamp() > oldStamp)
                changed.push_back(p);
        }
        else {
            failed.push_back(p);
        }
         
    }
            
}

bool Entity::executeActions(EntityPtr target,
                                           vector<EntityPtr>& allPre, 
                                           vector<EntityPtr>& changedPre,
                                           vector<EntityPtr>& failedPre
    )
{
    for (auto a : actions) {
        bool success = a->execute(target, allPre, changedPre, failedPre);
        if (!success)
            return false;
    }
    return true;
}

void Entity::show(ostream& os, int level, string indent)
{
    for (int i = 0; i < level; i++)
        os << indent;
    os << name() << endl;

    for (auto p : prerequisiteList)
        p->show(os, level + 1);
}
