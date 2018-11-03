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

bool PhonyEntity::update()
{
    // vector<EntityPtr> changed;
    // vector<EntityPtr> failed;
    DepInfo info;
    info.target = shared_from_this();
    info.all = prerequisiteList;
    updatePrerequisites(info);
    return executeActions(info);
}


time_t PhonyEntity::timestamp()
{
    return time(0);             // 当前时间
}
