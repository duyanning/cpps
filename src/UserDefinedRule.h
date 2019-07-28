#ifndef USERDEFINEDRULE_H
#define USERDEFINEDRULE_H

struct InfoPackageScanned;

// .cpp中的用户自定义规则
struct UserDefinedRule {
    fs::path dir; // 规则中的文件名，都是以该目录为当前目录的。命令也应在该目录中执行(fltk的fluid生成文件时，根本不管你指定的绝对路径，就直接生成在当前目录下了)。所以commands也应该在该目录中执行。
    vector<fs::path> targets;
    vector<fs::path> prerequisites;
    vector<string> commands;
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar& dir;
        ar&targets;
        ar& prerequisites;
        ar& commands;

    }
};

void process_user_defined_rule(fs::path src_path, string rule, InfoPackageScanned& pack);

#endif // USERDEFINEDRULE_H
