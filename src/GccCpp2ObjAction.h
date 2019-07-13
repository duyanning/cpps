#ifndef GCCCPP2OBJACTION_H
#define GCCCPP2OBJACTION_H

#include "Action.h"

class GccCpp2ObjAction : public Action {
public:
    bool execute(const DepInfo& info) override;
};

using GccCpp2ObjActionPtr = shared_ptr<GccCpp2ObjAction>;


inline
GccCpp2ObjActionPtr makeGccCpp2ObjAction()
{
    return GccCpp2ObjActionPtr(new GccCpp2ObjAction);
}

#endif // GCCCPP2OBJACTION_H
