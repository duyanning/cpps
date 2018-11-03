#include "config.h"
#include "Entity.h"
#include "DepInfo.h"
#include "Action.h"

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


void Entity::updatePrerequisites(DepInfo& info)
{
    for (auto p : prerequisiteList) {
        time_t oldStamp = p->timestamp();
        bool success = p->update();
        if (success) {
            if (p->timestamp() > oldStamp)
                info.changed.push_back(p);
        }
        else {
            info.failed.push_back(p);
        }
         
    }
            
}

bool Entity::executeActions(DepInfo& info)
{
    for (auto a : actions) {
        bool success = a->execute(info);
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
