#ifndef CMDLINEBUILDER_H
#define CMDLINEBUILDER_H


class CmdLineBuilder {
public:
    virtual void add_include_dirs(std::string& cmd, const std::vector<std::string> include_dirs) = 0;
    virtual void add_lib_dirs(std::string& cmd, const std::vector<std::string> lib_dirs) = 0;
	virtual void add_libs(std::string& cmd, const std::vector<std::string> libs) = 0;
	virtual void add_dll_dirs(std::string& cmd, const std::vector<std::string> dll_dirs) = 0;
	
};


using CmdLineBuilderPtr = std::shared_ptr<CmdLineBuilder>;


#endif // CMDLINEBUILDER_H
