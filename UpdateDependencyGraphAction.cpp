#include "std.h"
#include "UpdateDependencyGraphAction.h"

UpdateDependencyGraphAction::UpdateDependencyGraphAction(FileEntityPtr obj)
    :
    m_obj(obj)
{
}

UpdateDependencyGraphActionPtr makeUpdateDependencyGraphAction(FileEntityPtr obj)
{
    return UpdateDependencyGraphActionPtr(new UpdateDependencyGraphAction(obj));
}

void UpdateDependencyGraphAction::execute(EntityPtr target, vector<EntityPtr>&  allPre, vector<EntityPtr>& changedPre)
{
    FileEntityPtr dep = static_pointer_cast<FileEntity>(allPre[0]);
    fs::path dep_path = dep->path();

    ifstream f(dep_path.native());
    istream_iterator<string> i = istream_iterator<string>(f);

    // skip the first and second
    int count = 0;
    while (count < 2) {
        if (*i != "\\")
            count++;
        ++i;
    }

    for (; i != istream_iterator<string>(); ++i) {
        string t = *i;
        if (t != "\\") {
            fs::path p(t);
            m_obj->addPrerequisite(makeFileEntity(p));
        }
    }

}

