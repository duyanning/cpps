#ifndef ACTION_H
#define ACTION_H

#include "fwd.h"

class Action : public enable_shared_from_this<Action> {
public:
    // 返回值true表示动作执行成功，false表示失败
    virtual bool execute(const DepInfo& info) = 0;
    virtual ~Action() = 0;
};


#endif // ACTION_H
