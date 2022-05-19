#pragma once

#include "POWEngine/ECS/SystemBase.h"

namespace powe
{
	class Renderer;
	class RenderAPI;
	class RenderSystemBase : public SystemBase
	{
	public:

		virtual void InternalDraw(const RenderAPI& instance) = 0;
		void OnUpdate(float, powe::GameObjectID) override {}

	public:

		RenderSystemBase() = default;
		virtual ~RenderSystemBase() override = default;


	};

	template<typename T, typename = EnableIsBasedOf<RenderAPI,T>>
	class RenderSystem : public RenderSystemBase
	{
	public:

		void InternalDraw(const RenderAPI& instance) final;
		virtual void OnDraw(const T& renderer) = 0;

	public:

		RenderSystem() = default;
		virtual ~RenderSystem() override = default;
	};

	template <typename T, typename T0>
	void RenderSystem<T, T0>::InternalDraw(const RenderAPI& instance)
	{
		OnDraw(static_cast<const T&>(instance));
	}
}


