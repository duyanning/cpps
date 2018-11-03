#ifndef DEPINFO_H
#define DEPINFO_H

#include "fwd.h"
//class EntityPtr;

struct DepInfo {
    EntityPtr target;
    vector<EntityPtr> all;
    vector<EntityPtr> changed;
    vector<EntityPtr> failed;
};


#endif // DEPINFO_H
