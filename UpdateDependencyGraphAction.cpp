#include "std.h"
#include "UpdateDependencyGraphAction.h"

// def __init__(self, obj):
//     Action.__init__(self)
//     self.m_obj = obj
UpdateDependencyGraphAction::UpdateDependencyGraphAction(PFileEntity obj)
    :
    m_obj(obj)
{
}


void UpdateDependencyGraphAction::execute(PDependencyGraphEntity target,
                            vector<PDependencyGraphEntity>&  allPre, vector<PDependencyGraphEntity>& changedPre)
{
}

    // def execute(self, target, allPre, changedPre):
    //     Action.execute(self, target, allPre, changedPre)

    //     # analyze dependency file
    //     dep_path = allPre[0].path()
    //     f = open(dep_path, "r")
    //     nameList = []
    //     content = f.read()
    //     nameList = content.split()

    //     # skip the first and second
    //     for t in nameList[2:]:
    //         if t != "\\":
    //             self.m_obj.addPrerequisite(FileEntity(t, t))
