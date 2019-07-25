#include "config.h"
#include "VcCmdLineBuilder.h"

void
VcCmdLineBuilder::add_include_dirs(std::string& cmd, const std::vector<std::string> include_dirs)
{
    for (auto dir : include_dirs) {
        cmd += " /I";
        cmd += dir;
        //cout << dir << endl;
    }    
}

void
VcCmdLineBuilder::add_lib_dirs(std::string& cmd, const std::vector<std::string> lib_dirs)
{
    for (auto dir : lib_dirs) {
        cmd += " /LIBPATH:";
        cmd += dir;
        //cout << dir << endl;
    }    
}

void VcCmdLineBuilder::add_libs(std::string& cmd, const std::vector<std::string> libs)
{
	cmd += " ";
	for (auto lib : libs) {
		cmd += lib;
		cmd += ".lib ";
	}
}

void VcCmdLineBuilder::add_dll_dirs(std::string& cmd, const std::vector<std::string> dll_dirs)
{
	cmd = "PATH=";
	for (auto dir : dll_dirs) {
		cmd += dir;
		cmd += ";";
	}
}

