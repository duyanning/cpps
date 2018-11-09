#include "config.h"
#include "Entity.h"
#include "DepInfo.h"
#include "Action.h"
#include "Loggers.h"
#include "FileSig.h"
#include "FileEntity.h"

Entity::Entity()
{
}

Entity::~Entity()
{
}

void Entity::addAction(shared_ptr<Action> action)
{
    actions.push_back(action);
}

void Entity::addPrerequisite(EntityPtr p)
{
    prerequisiteList.push_back(p);
}

// 并不是发现下级节点比本节点更新时才重新生成本节点
// 而是当发现下级节点在更新后，比原来要新时，就重新生成本节点。
// 这样，就算copy来一个较新的.cpp覆盖原来的老.cpp，虽然这个新.cpp可能还没.o新，也会重新生成本节点
// 以上作废，说得不对
// 
void Entity::updatePrerequisites(DepInfo& info)
{
    for (auto p : prerequisiteList) {
        //time_t oldStamp = p->timestamp();
        // FileSig oldSig;
        // FileEntityPtr pFile;
        // bool hasSigFile;
        // if (p->isFile()) {
        //     pFile = static_pointer_cast<FileEntity>(p);
        //     hasSigFile = pFile->loadSig(oldSig);
        // }
        
        bool success = p->update();
        
        if (success) {
            // if (p->isFile()) {
            //     cout << "dddddddddddd\n";
            //     cout << p->name() << endl;
            //     FileSig newSig = pFile->sig();
            //     if (!hasSigFile || newSig != oldSig) //bug：导致第二次执行也要编译
            //     //if (hasSigFile && newSig != oldSig)
            //         info.changed.push_back(p);
            //     pFile->saveSig(newSig);
            // }
            // if (p->timestamp() > oldStamp) {
            //     info.changed.push_back(p);
            // }
        }
        else {
            info.failed.push_back(p);
        }
         
    }
            
}

bool Entity::executeActions(const DepInfo& info)
{
    // 如果就没关联什么动作，下级的更新失败，就是自己的失败
    if (actions.empty()) {
        if (!info.failed.empty())
            return false;
    }

    // 如果关联有动作，就让动作决定是否失败
    for (auto a : actions) {
        bool success = a->execute(info);
        if (!success) {
            MINILOG(update_logger, "Entity::executeActions FAILED: " << name());
            return false;
        }
    }
    MINILOG(update_logger, "Entity::executeActions OK: " << name());
    return true;
}

void Entity::show(ostream& os, int level, string indent)
{
    for (int i = 0; i < level; i++)
        os << indent;
    os << name() << endl;

    for (auto p : prerequisiteList)
        p->show(os, level + 1);
}
