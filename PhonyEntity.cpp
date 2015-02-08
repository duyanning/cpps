#include "std.h"
#include "PhonyEntity.h"


PhonyEntity::PhonyEntity(string name)
    :
    DependencyGraphEntity(name)
{
}

PhonyEntityPtr makePhonyEntity(string name)
{
    return PhonyEntityPtr(new PhonyEntity(name));
}

void PhonyEntity::update()
{
    vector<PDependencyGraphEntity> changed;
    updatePrerequisites(changed);
    executeActions(shared_from_this(), prerequisiteList, changed);
}


time_t PhonyEntity::timestamp()
{
    return time(0);
}
