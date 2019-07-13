#ifndef VCCPP2OBJACTION_H
#define VCCPP2OBJACTION_H

#include "Action.h"

class VcCpp2ObjAction : public Action {
	std::string m_additional_options;
	fs::path m_h_path; // 采用的预编译的头文件
public:
	VcCpp2ObjAction(std::string additional_options, fs::path h_path = "")
		: m_additional_options{ additional_options }, m_h_path{ h_path } 
	{

	}
	bool execute(const DepInfo& info) override;
};

using VcCpp2ObjActionPtr = shared_ptr<VcCpp2ObjAction>;


inline
VcCpp2ObjActionPtr makeVcCpp2ObjAction(std::string additional_options = "", fs::path h_path = "")
{
    return VcCpp2ObjActionPtr(new VcCpp2ObjAction(additional_options));
}

#endif // VCCPP2OBJACTION_H
