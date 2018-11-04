#include "config.h"
#include "Entity.h"
#include "DepInfo.h"
#include "Action.h"
#include "Loggers.h"

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

bool Entity::executeActions(const DepInfo& info)
{
    // 如果就没关联什么动作，下级的更新失败，就是自己的失败
    if (actions.empty()) {
        if (!info.failed.empty())
            return false;
    }

    // 如果关联有动作，就让动作决定是否失败
    for (auto a : actions) {
        bool success = a->execute(info);
        if (!success) {
            MINILOG(update_logger, "Entity::executeActions FAILED: " << name());
            return false;
        }
    }
    MINILOG(update_logger, "Entity::executeActions OK: " << name());
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
