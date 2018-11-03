#ifndef ACTION_H
#define ACTION_H

#include "fwd.h"

class Action : public enable_shared_from_this<Action> {
public:
    virtual bool execute(DepInfo& info) = 0;
    virtual ~Action() = 0;
};


#endif // ACTION_H
