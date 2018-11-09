#include "config.h"
#include "helpers.h"
#include "FileEntity.h" // using Entity.cpp
#include "Loggers.h"
#include "Birthcert.h"

FileEntity::FileEntity(fs::path path)
:
    m_path(path)
{
}

string FileEntity::name()
{
    return m_path.string();
}


fs::path FileEntity::path()
{
    return m_path;
}

bool FileEntity::update()
{
    MINILOG(update_logger, "Entity::update: " << name());
    // 如果一个文件节点是个叶子节点
    if (prerequisiteList.empty()) {
        return exists(m_path);  // 存在就好，不存在那就真没办法了
    }

    // 不是叶子节点的话，先把所有的下级节点更新一下
    DepInfo info;
    info.target = shared_from_this();
    info.all = prerequisiteList;

    updatePrerequisites(info);

    // 然后再判断是否要执行本节点关联的动作
    if (needExecuteActions(info)) {
        MINILOG(update_logger, "Entity::needExecuteActions: true " << name());
        return executeActions(info);
    }
        

    return true;
}

// bool FileEntity::needExecuteActions(const DepInfo& info)
// {
//     // 如果下级更新节点失败了，让本节点的动作决定如何处理
//     if (!info.failed.empty())
//         return true;

//     // 如果下级节点有改变，也要去执行动作
//     if (!info.changed.empty())
//         return true;

//     // 如果下级节点更新成功，并且无变化，就不需要执行本节点的动作
//     return false;
// }

// 若文件不存在，就认为它的生日为0
// 若它作为目标文件，生日为0意味着这是一个最古老的文件，它比任何依赖文件都要老，所以必须重新生成
// 若它作为依赖文件，生日0意味着文件不存在，没法用它生成目标文件
// todo：如果一个文件不存在，让这个函数抛出异常。
time_t FileEntity::timestamp()
{
    time_t timestamp;
    if (!exists(m_path))
        timestamp = 0;
    else {
        // timestamp is the modification time of file
        timestamp = last_write_time(m_path);
    }
    return timestamp;
}

FileSig FileEntity::sig()
{
    FileSig sig;
    sig.path = m_path;
    cout << "cccccccccccccc\n";
    sig.timestamp = last_write_time(m_path);
    sig.size = file_size(m_path);
    return sig;
}

// bool FileEntity::loadSig(FileSig& sig)
// {
//     fs::path sig_path = shadow(path());
//     sig_path += ".sig";

//     if (!exists(sig_path))
//         return false;

//     ifstream ifs(sig_path.string());
//     assert(ifs);

//     ifs >> sig;

//     return true;
// }

// void FileEntity::saveSig(const FileSig& sig)
// {
//     fs::path sig_path = shadow(path());
//     sig_path += ".sig";
//     cout << "haha: " << sig_path << endl;

//     ofstream ofs(sig_path.string());
//     assert(ofs);

//     ofs << sig;
// }


bool FileEntity::needExecuteActions(const DepInfo& info)
{
    // 如果本身还不存在，要重新生成
    if (!exists(path()))
        return true;

    // 如果有下级节点更新没成功，也去更新本节点，因为动作未必在乎
    if (!info.failed.empty())
        return true;

    // // 如果下级节点有变，它要重新生成
    // if (!info.changed.empty())
    //     return true;


    // 如果没有下级节点，就不需要更新本节点
    if (prerequisiteList.empty())
        return false;

    // 有下级节点的，还要看一下下级结点(即父母)跟自己出身证明上记录的是否一致
    // todo:此处应当从birthcert文件中读出hash表，然后从该表中查下级节点的签名跟当前签名进行比较
    // birthcert中记录的下级文件，可能少于.d中列出的下级文件。(这种情况发生在第一与第二次执行之间)
    
    // vector<FileSig> old_sig_vector;
    // this->get_src_sigs_from_birthcert(old_sig_vector);

    // vector<FileSig> new_sig_vector;
    // for (auto p : prerequisiteList) {
    //     FileEntityPtr fp = static_pointer_cast<FileEntity>(p);
    //     new_sig_vector.push_back(fp->sig());
    //     cout << "wwwwwwwwwwwww" << endl;
    // }

    // if (old_sig_vector != new_sig_vector) {
    //     MINILOGBLK(birthcert_logger,
    //                os << "old sig vector:\n";
    //                for (auto old_sig : old_sig_vector) {
    //                    os << old_sig;
    //                }
    //                os << "new sig vector:\n";
    //                for (auto new_sig : new_sig_vector) {
    //                    os << new_sig;
    //                }
    //         );
    //     return true;
    // }

    fs::path birthcert_path = path();
    birthcert_path += ".birthcert";

    ifstream ifs(birthcert_path.string());
    assert(ifs);

    boost::archive::text_iarchive ia(ifs);

    Birthcert birthcert;
    ia >> birthcert;
        
    vector<FileSig> new_sig_vector;
    for (auto p : prerequisiteList) {
        FileEntityPtr fp = static_pointer_cast<FileEntity>(p);

        if (birthcert.verifySig(fp->path().string(), fp->sig()) == false)
            return true;
    }
    
    return false;
}

// 从自己的出生证明文件中读取生成自己的源文件的签名
void FileEntity::get_src_sigs_from_birthcert(vector<FileSig>& sig_vector)
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
void FileEntity::generate_birth_cert(fs::path dep_path)
{
    vector<fs::path> pre_file_paths;
    get_pre_file_paths_from_dep_file(dep_path, pre_file_paths);

    // fs::path birthcert_path = this->path();
    // birthcert_path += ".birthcert";

    // ofstream ofs(birthcert_path.string());
    // ofs << pre_file_paths.size() << "\n";
    // for (auto src_path : pre_file_paths) {
    //     FileEntityPtr src = makeFileEntity(src_path);
    //     ofs << src->sig();
    // }
    generate_birth_cert(pre_file_paths);
}

void FileEntity::generate_birth_cert()
{
    vector<fs::path> pre_file_paths;
    for (auto p : prerequisiteList) {
        if (p->isFile()) {
            FileEntityPtr f = static_pointer_cast<FileEntity>(p);
            pre_file_paths.push_back(f->path());
        }
    }

    generate_birth_cert(pre_file_paths);
}

void FileEntity::generate_birth_cert(const vector<fs::path>& pre_file_paths)
{
    fs::path birthcert_path = this->path();
    birthcert_path += ".birthcert";

    // ofstream ofs(birthcert_path.string());
    // ofs << pre_file_paths.size() << "\n";
    // for (auto src_path : pre_file_paths) {
    //     FileEntityPtr src = makeFileEntity(src_path);
    //     ofs << src->sig();
    // }

    // todo：以上被注释部分应该为：构造一个hash表，key为文件路径，value为文件签名。
    // 然后将该hash表写入birthcert文件
    Birthcert birthcert;
    for (auto p : pre_file_paths) {
        FileEntityPtr f = makeFileEntity(p);
        birthcert.addSig(f->path().string(), f->sig());
    }

    ofstream ofs(birthcert_path.string());
    boost::archive::text_oarchive oa(ofs);
    oa << birthcert;
}

