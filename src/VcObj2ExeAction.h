#ifndef VCOBJ2EXEACTION_H
#define VCOBJ2EXEACTION_H

#include "Action.h"

class VcObj2ExeAction : public Action {
public:
    VcObj2ExeAction();
    bool execute(const DepInfo& info) override;
private:
};

using VcObj2ExeActionPtr = shared_ptr<VcObj2ExeAction>;

VcObj2ExeActionPtr makeVcObj2ExeAction();

#endif // VCOBJ2EXEACTION_H
