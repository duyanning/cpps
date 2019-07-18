#include "config.h"
#include "MingwCmdLineBuilder.h"

//void
//MingwCmdLineBuilder::add_include_dirs(std::string& cmd, const std::vector<std::string> include_dirs)
//{
//    for (auto dir : include_dirs) {
//        cmd += " -I";
//        cmd += dir;
//        //cout << dir << endl;
//    }    
//}
//
//void
//MingwCmdLineBuilder::add_lib_dirs(std::string& cmd, const std::vector<std::string> lib_dirs)
//{
//    for (auto dir : lib_dirs) {
//        cmd += " -L";
//        cmd += dir;
//        //cout << dir << endl;
//    }    
//}
//
//void MingwCmdLineBuilder::add_libs(std::string& cmd, const std::vector<std::string> libs)
//{
//	for (auto lib : libs) {
//		cmd += " -l";
//		cmd += lib;
//	}
//}

void MingwCmdLineBuilder::add_dll_dirs(std::string& cmd, const std::vector<std::string> dll_dirs)
{
	cmd = "PATH=";
	for (auto dir : dll_dirs) {
		cmd += dir;
		cmd += ";";
	}

}

