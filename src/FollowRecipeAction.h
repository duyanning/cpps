#ifndef FOLLOWRECIPEACTION_H
#define FOLLOWRECIPEACTION_H

#include "Action.h"
#include "UserDefinedRule.h"

class FollowRecipeAction : public Action {
public:
    FollowRecipeAction(const UserDefinedRule& rule)
        : m_rule{ rule }
    {}
    bool execute(const DepInfo& info) override;
private:
    UserDefinedRule m_rule;
    bool m_executed = false;
};

using FollowRecipeActionPtr = shared_ptr<FollowRecipeAction>;


inline
FollowRecipeActionPtr makeFollowRecipeAction(const UserDefinedRule& rule)
{
    return FollowRecipeActionPtr(new FollowRecipeAction(rule));
}


#endif // FOLLOWRECIPEACTION_H
