#ifndef DEPENDENCYGRAPHENTITY_H
#define DEPENDENCYGRAPHENTITY_H

class DependencyGraphEntity;
typedef shared_ptr<DependencyGraphEntity> PDependencyGraphEntity;

#include "Action.h"

class DependencyGraphEntity : public enable_shared_from_this<DependencyGraphEntity> {
public:
    DependencyGraphEntity(string name);
    void addAction(PAction action);
    void addPrerequisite(PDependencyGraphEntity p);
    virtual void update() = 0;
    void updatePrerequisites(vector<PDependencyGraphEntity>& changed);
    virtual time_t timestamp() = 0;
    string name();
    void executeActions(PDependencyGraphEntity target, vector<PDependencyGraphEntity>&  allPre, vector<PDependencyGraphEntity>& changedPre);
protected:
    vector<PDependencyGraphEntity> prerequisiteList;
    vector<PAction> actions;
private:
    string m_name;
};



#endif // DEPENDENCYGRAPHENTITY_H
