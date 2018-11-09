#include "config.h"
#include "PhonyEntity.h"
#include "DepInfo.h"

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
    DepInfo info;
    info.target = shared_from_this();
    info.all = prerequisiteList;
    updatePrerequisites(info);
    return executeActions(info);
}


// time_t PhonyEntity::timestamp()
// {
//     return time(0);             // 当前时间
// }
