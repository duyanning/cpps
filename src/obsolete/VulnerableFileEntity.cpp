#include "config.h"
#include "VulnerableFileEntity.h"
#include "Loggers.h"
#include "DepInfo.h"

VulnerableFileEntity::VulnerableFileEntity(fs::path path)
:
    FileEntity(path)
{
}


bool VulnerableFileEntity::needExecuteActions(const DepInfo& info)
{
    // 对于这种根据其他文件生成的文件

    // 如果有下级节点更新没成功，也去更新本节点，因为动作未必在乎
    if (!info.failed.empty())
        return true;

    // 如果下级节点有变，它要重新生成
    if (!info.changed.empty())
        return true;

    // 如果本身还不存在，要重新生成
    if (!exists(path()))
        return true;

    // 光如此还不够，还要看一下下级结点(即父母)跟自己出身证明上记录的是否一致
    vector<FileSig> old_sig_vector;
    this->get_src_sigs_from_birthcert(old_sig_vector);

    vector<FileSig> new_sig_vector;
    for (auto p : prerequisiteList) {
        FileEntityPtr fp = static_pointer_cast<FileEntity>(p);
        new_sig_vector.push_back(fp->sig());
    }

    if (old_sig_vector != new_sig_vector) {
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
        return true;
    }

    return false;
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
