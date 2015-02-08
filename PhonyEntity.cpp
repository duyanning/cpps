#include "std.h"
#include "PhonyEntity.h"


PhonyEntity::PhonyEntity(string name)
    :
    DependencyGraphEntity(name)
{
}


void PhonyEntity::update()
{
}


time_t PhonyEntity::timestamp()
{
    return time(0);
}
