#include "std.h"
#include "GchMagic.h"
#include "helpers.h"
#include "Loggers.h"

GchMagic::GchMagic(vector<fs::path>& headers_to_precompile)
    :
    m_headers_to_precompile(headers_to_precompile)
{
    MINILOG(gch_magic_logger, "move from shadow");
    for (auto h : m_headers_to_precompile) {
        fs::path gch = h;
        gch += ".gch";
        fs::path shadow_gch = shadow(gch);
        if (exists(shadow_gch)) {
            MINILOG(gch_magic_logger, shadow_gch << "\n=>\n" << gch);
            rename(shadow_gch, gch);
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
            rename(gch, shadow_gch);
        }
    }
    MINILOG(gch_magic_logger, "move to shadow - completed");
}
