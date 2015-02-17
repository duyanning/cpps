#include "std.h"
#include "VulnerableFileEntity.h"
#include "Loggers.h"

VulnerableFileEntity::VulnerableFileEntity(fs::path path)
:
    FileEntity(path)
{
}

VulnerableFileEntityPtr makeVulnerableFileEntity(fs::path path)
{
    return VulnerableFileEntityPtr(new VulnerableFileEntity(path));
}

void VulnerableFileEntity::update()
{
    // make all prerequisites
    vector<EntityPtr> changed;
    updatePrerequisites(changed);

    vector<FileSig> old_sig_vector;
    this->get_src_sigs_from_birthcert(old_sig_vector);

    vector<FileSig> new_sig_vector;
    // check to see if execution is needed
    bool needExecute = false;
    if (this->timestamp() == 0) {  // 如果目标文件不存在，那必须生成
        needExecute = true;
    }
    else {                      // 如果目标存在，根据依赖关系判断是否需要重新生成
        for (auto p : prerequisiteList) {
            if (p->timestamp() == 0) {
                string errMsg = name()  + ": cannot make `" + p->name() + "'";
                cout << errMsg << endl;
                throw 1;
            }
            FileEntityPtr fp = static_pointer_cast<FileEntity>(p);
            new_sig_vector.push_back(fp->sig());
        }
    }

    if (old_sig_vector != new_sig_vector) {
        needExecute = true;

        MINILOGBLK(birthcert_logger,
                   os << "old sig vector:\n";
                   for (auto old_sig : old_sig_vector) {
                       os << old_sig;
                   }
                   os << "new sig vector:\n";
                   for (auto new_sig : new_sig_vector) {
                       os << new_sig;
                   }
            );
    }

    // execute action
    if (needExecute)
        executeActions(shared_from_this(), prerequisiteList, changed);
}

// 从自己的出生证明文件中读取生成自己的源文件的签名
void VulnerableFileEntity::get_src_sigs_from_birthcert(vector<FileSig>& sig_vector)
{
    // 自己的出生证明文件总是跟自己在一起
    if (!exists(path()))
        return;

    // 获取自己的出生证明文件的路径
    fs::path birthcert_path = path();
    birthcert_path += ".birthcert";

    ifstream ifs(birthcert_path.string());
    assert(ifs);

    int total;
    ifs >> total;

    FileSig sig;
    for (int i = 0; i < total; i++) {
        ifs >> sig;
        sig_vector.push_back(sig);
    }

}

void get_pre_file_paths_from_dep_file(fs::path dep_path, vector<fs::path>& pre_file_paths)
{
    ifstream f(dep_path.string());
    assert(f);

    istream_iterator<string> i = istream_iterator<string>(f);

    // skip the first
    int count = 0;
    while (count < 1) {
        if (*i != "\\")
            count++;
        ++i;
    }

    for (; i != istream_iterator<string>(); ++i) {
        string t = *i;
        if (t != "\\") {
            fs::path p(t);
            pre_file_paths.push_back(p);
        }
    }
}


// （根据.d文件中指出的依赖关系）生成出生证明文件
void VulnerableFileEntity::generate_birth_cert(fs::path dep_path)
{
    vector<fs::path> pre_file_paths;
    get_pre_file_paths_from_dep_file(dep_path, pre_file_paths);

    fs::path birthcert_path = this->path();
    birthcert_path += ".birthcert";

    ofstream ofs(birthcert_path.string());
    ofs << pre_file_paths.size() << "\n";
    for (auto src_path : pre_file_paths) {
        FileEntityPtr src = makeFileEntity(src_path);
        ofs << src->sig();
    }
}
