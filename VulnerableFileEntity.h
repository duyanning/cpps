#ifndef VULNERABLEFILEENTITY_H
#define VULNERABLEFILEENTITY_H

#include "FileEntity.h"

// .o文件（类似的还有.gch文件）是从用户提供的.cpp文件、.h文件直接生成的。
// 而.cpp、.h文件并不在cpps的管理之下（只有cache目录下的东西是在cpps的
// 管理之下，如用于生成exe文件的.o文件），所以在由.cpp文件、.h文件生
// 成.o文件时，仅仅根据时间戳来决定是否重新生成是不够的。比如：你有一个
// hello.cpp（老内容），修改时间为2015-01-01，你将它复制到另外一个目录，
// 并对其进行修改，修改后的hello.cpp（新内容）的修改时间假设为
// 2015-01-03，然后你编译这个修改后的hello.cpp（新内容）得到hello.o。此
// 时，你删除这个hello.cpp（新内容），并将最早那个hello.cpp（老内容）移
// 动过来，因为移动并不会改变文件的修改时间，所以此时，hello.cpp（老内
// 容）的时间戳早于hello.o，仅仅根据时间戳并不会重新生成hello.o，这显然
// 不是我们想要的。对于这种情况，时间戳是不够用的，要判断是否需要重新生
// 成目标文件，得以生成目标文件所依赖的源文件（可将目标文件比喻为孩子，
// 将其依赖的源文件比喻为父母）是否发生变化为依据。所以我们在生成目标文
// 件时，将其依赖的源文件的签名记录在目标文件的出生证明文件（.birthcert）
// 中，下次判断是否需要重新生成该目标文件时，就检查出生证明中记录的父母
// 签名跟新的父母签名是否一致，不一致就要重新生成。
class VulnerableFileEntity : public FileEntity {
public:
    VulnerableFileEntity(fs::path path);
    void update() override;
    void get_src_sigs_from_birthcert(vector<FileSig>& sig_vector);
    void generate_birth_cert(fs::path dep_path);
};

using VulnerableFileEntityPtr = shared_ptr<VulnerableFileEntity>;
VulnerableFileEntityPtr makeVulnerableFileEntity(fs::path path);

#endif // VULNERABLEFILEENTITY_H
