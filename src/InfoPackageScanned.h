#ifndef INFOPACKAGESCANNED_H
#define INFOPACKAGESCANNED_H

//#include "FileSig.h"
#include "UserDefinedRule.h"

namespace boost {
    namespace serialization {

        template <typename Archive>
        void serialize(Archive& ar, fs::path& p, const unsigned int version)
        {
            string s;
            if (Archive::is_saving::value)
                s = p.string();
            //ar& boost::serialization::make_nvp("string", s); // https://stackoverflow.com/questions/8532331/what-does-boost-serialization-nvp-do-when-serializing-object
            ar& s;
            if (Archive::is_loading::value)
                p = s;
        }

    }
}

struct InfoPackageScanned {
    //FileSig cpp_sig; // .cpp文件的签名
    string cpp_sig; // .cpp文件的签名

    vector<fs::path> referenced_sources; // 本.cpp引用的其他.cpp
    vector<string> referenced_libs; // 本.cpp通过linklib或<compiler>-linklib引用的库名字
    unordered_map<string, string> referenced_compiler_specific_extra_compile_flags; // 本.cpp通过<compiler>-extra-compile-flags指定的额外编译选项
    string referenced_compiler_specific_extra_link_flags; // 本.cpp通过<compiler>-extra-link-flags指定的额外链接选项
    vector<fs::path> referenced_headers_to_pc; // 本.cpp通过precompile指定的需要预编译的头文件

    bool referenced_vc_use_pch = false;
    fs::path referenced_vc_h_to_precompile;
    fs::path referenced_vc_cpp_to_generate_pch;

    vector<UserDefinedRule> user_defined_rules;

    vector<fs::path> generated_files; // 这些文件是用户自定义规则生成的，所以我们不检查它们是否存在

    void clear()
    {
        InfoPackageScanned new_obj;
        *this = new_obj;
    }

private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar& cpp_sig;
        ar& referenced_sources;
        ar& referenced_libs;
        ar & referenced_compiler_specific_extra_compile_flags;
        ar & referenced_compiler_specific_extra_link_flags;
        ar & referenced_headers_to_pc;
        ar & referenced_vc_use_pch;
        ar & referenced_vc_h_to_precompile;
        ar & referenced_vc_cpp_to_generate_pch;
        ar& user_defined_rules;
        ar& generated_files;
    }

};


#endif // INFOPACKAGESCANNED_H
