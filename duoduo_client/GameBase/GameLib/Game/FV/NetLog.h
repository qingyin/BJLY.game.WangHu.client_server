#pragma once
#include <string>
#include "Game/Script/UtilityBase.h"

#define NetAssert(bAssert,kStr) CCAssert(bAssert,kStr);

namespace NetLog
{
	void Log(std::string kStr); 
}