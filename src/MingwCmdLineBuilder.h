#ifndef MINGWCMDLINEBUILDER_H
#define MINGWCMDLINEBUILDER_H

#include "GccCmdLineBuilder.h"

class MingwCmdLineBuilder : public GccCmdLineBuilder {
public:
 //   void add_include_dirs(std::string& cmd, const std::vector<std::string> include_dirs) override;
 //   void add_lib_dirs(std::string& cmd, const std::vector<std::string> lib_dirs) override;
	//void add_libs(std::string& cmd, const std::vector<std::string> libs) override;
	void add_dll_dirs(std::string& cmd, const std::vector<std::string> dll_dirs) override;
};


using MingwCmdLineBuilderPtr = std::shared_ptr<MingwCmdLineBuilder>;

inline
CmdLineBuilderPtr makeMingwCmdLineBuilder()
{
    return MingwCmdLineBuilderPtr(new MingwCmdLineBuilder);
}



#endif // MINGWCMDLINEBUILDER_H
