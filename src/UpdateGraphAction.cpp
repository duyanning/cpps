#include "config.h"
#include "UpdateGraphAction.h"

UpdateGraphAction::UpdateGraphAction(FileEntityPtr obj)
    :
    m_obj(obj)
{
}

UpdateGraphActionPtr makeUpdateGraphAction(FileEntityPtr obj)
{
    return UpdateGraphActionPtr(new UpdateGraphAction(obj));
}

bool UpdateGraphAction::execute(const DepInfo& info)
{
    FileEntityPtr dep = static_pointer_cast<FileEntity>(info.all[0]);
    fs::path dep_path = dep->path();

    ifstream f(dep_path.string());
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
    return true;
}

