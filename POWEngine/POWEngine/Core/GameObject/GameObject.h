
#pragma once

#include "POWEngine/ECS/ECSTypes.h"
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

		// TODO: Decide how long would gameobject still be alive after lose pointer reference
		~GameObject(); 

	public:

		GameObjectID GetID() const { return m_Id; }
		WorldEntity& GetWorld() const { return m_World; }

		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> GetComponent();

		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> AddComponent(
			ComponentType&& component,
			ComponentFlag flag = ComponentFlag::Default);

		template<typename ComponentType>
		void RemoveComponent() const;

	private:

		WorldEntity& m_World; // GameObject cannot exist without world entity
		GameObjectID m_Id;
	};

	template <typename ComponentType>
	EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> GameObject::GetComponent()
	{
		return m_World.GetComponent<ComponentType>(m_Id);
	}

	template <typename ComponentType>
	EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> GameObject::AddComponent(ComponentType&& component,ComponentFlag flag)
	{
		return m_World.AddComponentToGameObject(m_Id, std::move(component), flag);
	}

	template <typename ComponentType>
	void GameObject::RemoveComponent() const
	{
		m_World.RemoveComponentByType<ComponentType>(m_Id);
	}
}


