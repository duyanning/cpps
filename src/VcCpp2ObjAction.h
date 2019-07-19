#ifndef VCCPP2OBJACTION_H
#define VCCPP2OBJACTION_H

#include "Action.h"

class VcCpp2ObjAction : public Action {
	//std::string m_additional_options;
	//fs::path m_h_path; // 采用的预编译的头文件
public:
	VcCpp2ObjAction()
	{

	}
	bool execute(const DepInfo& info) override;
};

using VcCpp2ObjActionPtr = shared_ptr<VcCpp2ObjAction>;


inline
VcCpp2ObjActionPtr makeVcCpp2ObjAction()
{
    return VcCpp2ObjActionPtr(new VcCpp2ObjAction);
}

#endif // VCCPP2OBJACTION_H
