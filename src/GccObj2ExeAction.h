#ifndef GCCOBJ2EXEACTION_H
#define GCCOBJ2EXEACTION_H

#include "Action.h"

class GccObj2ExeAction : public Action {
public:
    GccObj2ExeAction(string other_options);
    bool execute(const DepInfo& info) override;
private:
    string m_other_options;
};

using GccObj2ExeActionPtr = shared_ptr<GccObj2ExeAction>;

GccObj2ExeActionPtr makeGccObj2ExeAction(string other_options);

#endif // GCCOBJ2EXEACTION_H
