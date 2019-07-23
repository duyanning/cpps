#ifndef GCCCMDLINEBUILDER_H
#define GCCCMDLINEBUILDER_H

#include "CmdLineBuilder.h"

class GccCmdLineBuilder : public CmdLineBuilder {
public:
    void add_include_dirs(std::string& cmd, const std::vector<std::string> include_dirs) override;
    void add_lib_dirs(std::string& cmd, const std::vector<std::string> lib_dirs) override;
	void add_libs(std::string& cmd, const std::vector<std::string> libs) override;
	void add_dll_dirs(std::string& cmd, const std::vector<std::string> dll_dirs) override;
};


using GccCmdLineBuilderPtr = std::shared_ptr<GccCmdLineBuilder>;

inline
CmdLineBuilderPtr makeGccCmdLineBuilder()
{
    return GccCmdLineBuilderPtr(new GccCmdLineBuilder);
}



#endif // GCCCMDLINEBUILDER_H
