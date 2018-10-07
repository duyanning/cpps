#include "config.h"
#include "PhonyEntity.h"


PhonyEntity::PhonyEntity(string name)
    :
    m_name(name)
{
}

string PhonyEntity::name()
{
    return m_name;
}

PhonyEntityPtr makePhonyEntity(string name)
{
    return PhonyEntityPtr(new PhonyEntity(name));
}

void PhonyEntity::update()
{
    vector<EntityPtr> changed;
    updatePrerequisites(changed);
    executeActions(shared_from_this(), prerequisiteList, changed);
}


time_t PhonyEntity::timestamp()
{
    return time(0);             // 当前时间
}
