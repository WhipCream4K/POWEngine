#pragma once

#include <concepts>

namespace powe
{
	class Layer
	{
	public:
		
		Layer() = default;
		virtual ~Layer() = default;

		virtual void OnAttach() = 0;
		virtual void OnUpdate(float delta) = 0;
		virtual void OnDetach() = 0;
		virtual void OnRender() = 0;
	};
}