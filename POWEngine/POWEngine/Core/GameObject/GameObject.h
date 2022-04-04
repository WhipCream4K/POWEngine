#pragma once

#include "POWEngine/Core/Components/BaseComponent.h"
#include "POWEngine/Core/ECS/ECSTypes.h"

namespace powe
{
	class WorldEntity;
	class GameObject final // Node
	{
	public:

		explicit GameObject(WorldEntity& world);
		GameObject(const GameObject&) = delete;
		GameObject& operator=(const GameObject&) = delete;
		GameObject(GameObject&&) noexcept = delete;
		GameObject& operator=(GameObject&&) noexcept = delete;
		~GameObject() = default;

	public:

		GameObjectID GetID() const { return m_Id; }

		//template<typename ComponentType>
		//EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> AddComponent(ComponentType&& component);

		//template<typename ComponentType>
		//EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> AddComponentSparse(ComponentType&& component);


	private:

		WorldEntity& m_World; // GameObject cannot exist without world entity
		GameObjectID m_Id;
	};
}


