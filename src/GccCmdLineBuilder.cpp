#include "config.h"
#include "GccCmdLineBuilder.h"

void
GccCmdLineBuilder::add_include_dirs(std::string& cmd, const std::vector<std::string> include_dirs)
{
    for (auto dir : include_dirs) {
        cmd += " -I";
        cmd += dir;
        //cout << dir << endl;
    }    
}

void
GccCmdLineBuilder::add_lib_dirs(std::string& cmd, const std::vector<std::string> lib_dirs)
{
    for (auto dir : lib_dirs) {
        cmd += " -L";
        cmd += dir;
        //cout << dir << endl;
    }    
}

