#ifndef ENTITY_H
#define ENTITY_H

#include "Action.h"
#include "DepInfo.h"

// class Entity;
// using EntityPtr = shared_ptr<Entity>;

class Entity : public enable_shared_from_this<Entity> {
public:
    Entity();
    virtual ~Entity() = 0;
    void addAction(ActionPtr action);
    void addPrerequisite(EntityPtr p);
    virtual bool update() = 0;
    //void updatePrerequisites(vector<EntityPtr>& changed, vector<EntityPtr>& failed);
    void updatePrerequisites(DepInfo& info);
    virtual time_t timestamp() = 0; // 相当于文件的生日，数字越小，文件越老。
    virtual string name() = 0;
    //void executeActions(EntityPtr target, vector<EntityPtr>&  allPre, vector<EntityPtr>& changedPre);
    bool executeActions(DepInfo& info);
    // bool executeActions(EntityPtr target,
    //                     vector<EntityPtr>& allPre,
    //                     vector<EntityPtr>& changedPre,
    //                     vector<EntityPtr>& failedPre);
    void show(ostream& os, int level = 0, string indent = "  ");
protected:
    vector<EntityPtr> prerequisiteList;
    vector<ActionPtr> actions;
private:
};



#endif // ENTITY_H
