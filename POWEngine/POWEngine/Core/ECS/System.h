#pragma once
#include "SystemBase.h"

namespace powe
{
	template<typename ...Args>
	class System :
		public SystemBase
	{
	public:

		System(const SharedPtr<WorldEntity>& worldEntt, PipelineLayer layer);
	};

	template <typename ... Args>
	System<Args...>::System(const SharedPtr<WorldEntity>& worldEntt, PipelineLayer layer)
		: SystemBase(worldEntt, layer)
	{
	}
}


