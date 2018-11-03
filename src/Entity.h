#ifndef ENTITY_H
#define ENTITY_H

#include "fwd.h"

class Entity : public enable_shared_from_this<Entity> {
public:
    Entity();
    virtual ~Entity() = 0;
    void addAction(ActionPtr action);
    void addPrerequisite(EntityPtr p);
    virtual bool update() = 0;
    void updatePrerequisites(DepInfo& info);
    virtual time_t timestamp() = 0; // 相当于文件的生日，数字越小，文件越老。
    virtual string name() = 0;
    bool executeActions(DepInfo& info);
    void show(ostream& os, int level = 0, string indent = "  ");
protected:
    vector<EntityPtr> prerequisiteList;
    vector<ActionPtr> actions;
private:
};



#endif // ENTITY_H
