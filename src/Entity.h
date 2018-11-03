#ifndef ENTITY_H
#define ENTITY_H

#include "fwd.h"

class Entity : public enable_shared_from_this<Entity> {
public:
    Entity();
    virtual ~Entity() = 0;
    void addAction(ActionPtr action);
    void addPrerequisite(EntityPtr p);
    // 返回值true/false表示更新成功/失败
    virtual bool update() = 0;
    // 更新下级节点
    void updatePrerequisites(DepInfo& info);
    virtual time_t timestamp() = 0; // 相当于文件的生日，数字越小，文件越老。
    virtual string name() = 0;
    // 执行所有关联的动作
    bool executeActions(const DepInfo& info);
    void show(ostream& os, int level = 0, string indent = "  ");
protected:
    vector<EntityPtr> prerequisiteList; // 所有下级节点
    vector<ActionPtr> actions;  // 所有关联的动作
private:
};



#endif // ENTITY_H
