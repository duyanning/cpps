#ifndef ENTITY_H
#define ENTITY_H

#include "fwd.h"

class Entity : public enable_shared_from_this<Entity> {
public:
    Entity();
    virtual ~Entity() = 0;

    // 关联一个动作(可以关联好多个)
    void addAction(ActionPtr action);

    // 添加一个下级节点(当前节点依赖于下级节点)
    void addPrerequisite(EntityPtr p);

    // 更新当前节点。返回值true/false表示更新成功/失败
    virtual bool update() = 0;

    //virtual time_t timestamp() = 0; // 相当于文件的生日，数字越小，文件越老。
    virtual string name() = 0;
    virtual bool isFile() = 0;

    // 执行所有关联的动作
    bool executeActions(const DepInfo& info);

    void show(ostream& os, int level = 0, string indent = "  ");
protected:
    // 更新所有下级节点，并根据更新的结果填充info：确定哪些下级节点更
    // 新后变年轻了，哪些下级节点更新失败了
    void updatePrerequisites(DepInfo& info);
    vector<EntityPtr> prerequisiteList; // 所有下级节点
    vector<ActionPtr> actions;  // 所有关联的动作
private:
};



#endif // ENTITY_H
