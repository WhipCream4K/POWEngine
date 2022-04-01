#pragma once

#include "POWEngine/Core/Components/BaseComponent.h"
#include "POWEngine/Core/ECS/ECSTypes.h"

namespace powe
{
	class WorldEntity;
	class GameObject final
	{
	public:

		explicit GameObject(const SharedPtr<WorldEntity>& world);
		GameObject(const GameObject&) = delete;
		GameObject& operator=(const GameObject&) = delete;
		GameObject(GameObject&&) noexcept = delete;
		GameObject& operator=(GameObject&&) noexcept = delete;
		~GameObject() = default;

	public:

		GameObjectId GetID() const { return m_Id; }

		//template<typename ComponentType>
		//EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> AddComponent(ComponentType&& component);

		//template<typename ComponentType>
		//EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> AddComponentSparse(ComponentType&& component);
		
		void SetParent(const SharedPtr<GameObject>& object);


	private:

		WeakPtr<WorldEntity> m_World; // GameObject cannot exist without world entity
		GameObjectId m_Parent;
		std::vector<ComponentTypeID> m_ChildrenComponents;
		GameObjectId m_Id;
	};
}


