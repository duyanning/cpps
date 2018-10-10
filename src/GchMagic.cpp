#include "config.h" // precompile
#include "GchMagic.h"
#include "helpers.h"
#include "Loggers.h"

// gcc使用.gch文件时，.gch文件必须跟.h文件位于同一目录下。然而，我们不
// 能在.h文件所在的目录下留下.gch文件，因为这样会弄脏.h文件所在的目录。
// 所以我们想了一个办法：我们就将.gch文件生成在.h文件所在的目录中。当
// gcc使用完.gch文件之后，我们将其移动到我们的影子目录中；在gcc使用.gch之
// 前，将其从影子目录移动到.h文件所在的目录下。

GchMagic::GchMagic(vector<fs::path>& headers_to_precompile)
    :
    m_headers_to_precompile(headers_to_precompile)
{
    MINILOG(gch_magic_logger, "move from shadow");
    for (auto h : m_headers_to_precompile) {
        fs::path gch = h;
        gch += ".gch";
        fs::path shadow_gch = shadow(gch);

        // 跟其他编译系统生成的预编译头文件和谐相处，暂时将其改名
        if (exists(gch)) {
            fs::path other_gch = gch;
            other_gch += ".old";
            rename(gch, other_gch);
        }

        // 因为运行cpps的用户不一定有在.h所在目录下产生文件（.gch）的权
        // 限，所以即便用户要求对某个.h文件进行预编译，也不一定能产生对
        // 应的.gch文件。但.gch文件只要存在，其出生证明文件就存在
        if (exists(shadow_gch)) {
            MINILOG(gch_magic_logger, shadow_gch << "\n=>\n" << gch);


            move(shadow_gch, gch);

            fs::path gch_birthcert = gch;
            gch_birthcert += ".birthcert";
            fs::path shadow_gch_birthcert = shadow_gch;
            shadow_gch_birthcert += ".birthcert";
            move(shadow_gch_birthcert, gch_birthcert);

        }
    }

    MINILOG(gch_magic_logger, "move from shadow - completed");
}

GchMagic::~GchMagic()
{
    MINILOG(gch_magic_logger, "move to shadow");
    for (auto h : m_headers_to_precompile) {
        fs::path gch = h;
        gch += ".gch";
        fs::path shadow_gch = shadow(gch);

        if (exists(gch)) {
            MINILOG(gch_magic_logger, gch << "\n=>\n" << shadow_gch);
            move(gch, shadow_gch);

            fs::path gch_birthcert = gch;
            gch_birthcert += ".birthcert";
            fs::path shadow_gch_birthcert = shadow_gch;
            shadow_gch_birthcert += ".birthcert";
            move(gch_birthcert, shadow_gch_birthcert);
        }

        // 跟其他编译系统生成的目录形式的预编译头文件和谐相处，恢复其原名
        fs::path other_gch = gch;
        other_gch += ".old";
        if (exists(other_gch)) {
            rename(other_gch, gch);
        }

    }


    MINILOG(gch_magic_logger, "move to shadow - completed");
}
