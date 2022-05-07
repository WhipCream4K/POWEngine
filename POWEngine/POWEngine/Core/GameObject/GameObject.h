#pragma once

#include "POWEngine/Core/Components/BaseComponent.h"
#include "POWEngine/Core/ECS/ECSTypes.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"

namespace powe
{
	class GameObject final
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
		WorldEntity& GetWorld() const { return m_World; }

		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> GetComponent();

		//template<typename ComponentType>
		//EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> AddComponent(ComponentType&& component);

		//template<typename ComponentType>
		//EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> AddComponentSparse(ComponentType&& component);


	private:

		WorldEntity& m_World; // GameObject cannot exist without world entity
		GameObjectID m_Id;
	};

	template <typename ComponentType>
	EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> GameObject::GetComponent()
	{
		return m_World.GetComponent<ComponentType>(m_Id);
	}
}


