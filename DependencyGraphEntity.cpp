#include "std.h"
#include "DependencyGraphEntity.h"

DependencyGraphEntity::DependencyGraphEntity(string name)
    :
    m_name(name)
{
}

string DependencyGraphEntity::name()
{
    return m_name;
}

void DependencyGraphEntity::addAction(shared_ptr<Action> action)
{
    actions.push_back(action);
}

void DependencyGraphEntity::addPrerequisite(PDependencyGraphEntity p)
{
    prerequisiteList.push_back(p);
}


void DependencyGraphEntity::updatePrerequisites(vector<PDependencyGraphEntity>& changed)
{
    for (auto p : prerequisiteList) {
        time_t oldStamp = p->timestamp();
        p->update();
        if (p->timestamp() > oldStamp)
            changed.push_back(p);
    }
            
}

void DependencyGraphEntity::executeActions(PDependencyGraphEntity target, vector<PDependencyGraphEntity>&  allPre, vector<PDependencyGraphEntity>& changedPre)
{
    for (auto a : actions)
        a->execute(target, allPre, changedPre);
}

    // def show(self, level=0, indent="  "):
    //     print level*indent, self.name()

    //     for p in self.prerequisiteList:
    //         p.show(level+1)

void DependencyGraphEntity::show(int level, string indent)
{
    for (int i = 0; i < level; i++)
        cout << indent;
    cout << name() << endl;

    for (auto p : prerequisiteList)
        p->show(level + 1);
}

