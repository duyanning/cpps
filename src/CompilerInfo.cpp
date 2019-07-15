#include "config.h"
#include "CompilerInfo.h"

CC cc = GCC;

CompilerInfo cc_info[] = {
	{
        "gcc",
		".o", 
		".gch"
	}, 
	{
        "vc",
		".obj",
		".pch"
	}
};
