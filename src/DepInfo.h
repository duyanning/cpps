#ifndef DEPINFO_H
#define DEPINFO_H

#include "fwd.h"
//class EntityPtr;

struct DepInfo {
    EntityPtr target;           // 目标
    vector<EntityPtr> all;      // 所有下级节点
    //vector<EntityPtr> changed;  // 改变的下级节点(是否改变，要跟当前节点上次生成时所使用的进行比较)
    vector<EntityPtr> failed;   // 失败的下级节点
};


#endif // DEPINFO_H
