#pragma once

#include "POWEngine/Core/CustomTypes.h"
#include "POWEngine/Core/Thread/SimpleThreadPool.h"

namespace powe
{
	class Logger
	{
	public:

		Logger(const SimpleThreadPool& pool);
		
	private:

		//static std::vector<std::string> MessageStack;
	};
}


