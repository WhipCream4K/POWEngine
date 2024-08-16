#pragma once

#include "WindowManager.h"

namespace powe
{
	class Layer
	{
	public:
		
		Layer() = default;
		Layer(const Layer&) = default;
		Layer& operator=(const Layer&) = default;
		Layer(Layer&&) = default;
		Layer& operator=(Layer&&) = default;
		virtual ~Layer() = default;

		virtual void OnAttach() = 0;
		virtual void OnUpdate(float) = 0;
		virtual void OnWindowEvents(const Window::EventQueue&) = 0;
		virtual void OnDetach() = 0;
	};

	template<typename T>
	concept LayerConcept = std::is_base_of_v<Layer, T>;
}
