#ifndef VCCMDLINEBUILDER_H
#define VCCMDLINEBUILDER_H

#include "CmdLineBuilder.h"

class VcCmdLineBuilder : public CmdLineBuilder {
public:
    void add_include_dirs(std::string& cmd, const std::vector<std::string> include_dirs) override;
    void add_lib_dirs(std::string& cmd, const std::vector<std::string> lib_dirs) override;
};

using VcCmdLineBuilderPtr = std::shared_ptr<VcCmdLineBuilder>;

inline
VcCmdLineBuilderPtr makeVcCmdLineBuilder()
{
    return VcCmdLineBuilderPtr(new VcCmdLineBuilder);
}



#endif // VCCMDLINEBUILDER_H
