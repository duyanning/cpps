#ifndef CPP2OBJACTION_H
#define CPP2OBJACTION_H

#include "Action.h"

class Cpp2ObjAction : public Action {
public:
    bool execute(const DepInfo& info) override;
};

using Cpp2ObjActionPtr = shared_ptr<Cpp2ObjAction>;


inline
Cpp2ObjActionPtr makeCpp2ObjAction()
{
    return Cpp2ObjActionPtr(new Cpp2ObjAction);
}

#endif // CPP2OBJACTION_H
