#pragma once

#include "POWEngine/Core/Components/BaseComponent.h"

namespace powe
{
	class Draw2DObject
	{
	public:

		Draw2DObject() = default;
		virtual int GetRenderOrder() const = 0;
		virtual void SetRenderOrder(int order) = 0;
		virtual ~Draw2DObject() = default;
	};
}



