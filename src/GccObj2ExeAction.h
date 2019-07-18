#ifndef GCCOBJ2EXEACTION_H
#define GCCOBJ2EXEACTION_H

#include "Action.h"

class GccObj2ExeAction : public Action {
public:
    GccObj2ExeAction();
    bool execute(const DepInfo& info) override;
private:
};

using GccObj2ExeActionPtr = shared_ptr<GccObj2ExeAction>;

GccObj2ExeActionPtr makeGccObj2ExeAction();

#endif // GCCOBJ2EXEACTION_H
