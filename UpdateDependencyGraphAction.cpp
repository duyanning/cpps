#include "std.h"
#include "UpdateDependencyGraphAction.h"

UpdateDependencyGraphAction::UpdateDependencyGraphAction(FileEntityPtr obj)
    :
    m_obj(obj)
{
}


void UpdateDependencyGraphAction::execute(PDependencyGraphEntity target,
                            vector<PDependencyGraphEntity>&  allPre, vector<PDependencyGraphEntity>& changedPre)
{
    FileEntityPtr dep = static_pointer_cast<FileEntity>(allPre[0]);
    fs::path dep_path = dep->path();

    ifstream f(dep_path.native());
    istream_iterator<string> i = istream_iterator<string>(f);
    // skip the first and second
    ++i;
    ++i;

    for (; i != istream_iterator<string>(); ++i) {
        string t = *i;
        if (t != "\\") {
            fs::path p(t);
            m_obj->addPrerequisite(FileEntityPtr(new FileEntity(t, p)));
        }
    }

}

