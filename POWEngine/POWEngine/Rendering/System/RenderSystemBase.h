#pragma once

#include "POWEngine/ECS/SystemBase.h"

namespace powe
{
	class Renderer;
	class RenderAPI;
	class RenderSystemBase
	{
	public:

		virtual void InternalDraw(const WorldEntity& worldEntity,const Archetype&,const RenderAPI&) = 0;

	public:

		const std::unordered_set<ComponentTypeID>& GetKeys() const { return m_Keys; }

	public:

		RenderSystemBase() = default;
		virtual ~RenderSystemBase() = default;

	protected:

		template<typename ...Args>
		void InternMakeKeys();

	private:

		std::unordered_set<powe::ComponentTypeID> m_Keys;


	};

	template<bool U,typename = std::enable_if_t<U,void>>
	class IfBase{};

	template<typename T>
	class RenderSystem : public RenderSystemBase , public IfBase<std::is_base_of_v<RenderAPI,T>>
	{
	public:

		void InternalDraw(const WorldEntity& worldEntity, const Archetype&, const RenderAPI&) final;
		virtual void OnDraw(const WorldEntity& worldEntity,const T& renderer,GameObjectID id) = 0;

	public:

		RenderSystem() = default;
		virtual ~RenderSystem() override = default;

	private:

		std::unordered_set<powe::ComponentTypeID> m_Keys;

	};

	template <typename ... Args>
	void RenderSystemBase::InternMakeKeys()
	{
		m_Keys = { BaseComponent::GetId<Args>()... };
	}

	template <typename T>
	void RenderSystem<T>::InternalDraw(const WorldEntity& worldEntity, const Archetype& archetype, const RenderAPI& renderer)
	{
		for (const GameObjectID gameObjectID: archetype.GameObjectIds)
		{
			OnDraw(worldEntity, static_cast<const T&>(renderer), gameObjectID);
		}
	}


#pragma region MACRO

#define DEFINE_SYSTEM_KEY(...) InternMakeKeys<__VA_ARGS__>()

#pragma endregion
}


