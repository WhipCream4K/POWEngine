#pragma once

#include <concepts>

namespace powe
{
	class Layer
	{
	public:
		
		Layer() = default;
		Layer(const Layer&) = delete;
		Layer& operator=(const Layer&) = delete;
		Layer(Layer&&) = delete;
		Layer& operator=(Layer&&) = delete;
		virtual ~Layer() = default;

		virtual void OnAttach() = 0;
		virtual void OnUpdate(float delta) = 0;
		virtual void OnDetach() = 0;
		virtual void OnRender() = 0;
	};

	template<typename T>
	concept LayerConcept = std::is_base_of_v<Layer, T>;
}