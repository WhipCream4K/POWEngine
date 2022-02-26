#pragma once

#include "POWEngine/Core/Components/BaseComponent.h"
#include "POWEngine/Core/ECS/ECSTypes.h"

namespace powe
{
	class WorldEntity;
	class GameObject final
	{
	public:

		GameObject(const SharedPtr<WorldEntity>& world);
		GameObject(const GameObject&) = default;
		GameObject& operator=(const GameObject&) = default;
		GameObject(GameObject&&) noexcept = default;
		GameObject& operator=(GameObject&&) noexcept = default;

	public:

		//template<typename ComponentType>
		//EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> AddComponent(ComponentType&& component);

		//template<typename ComponentType>
		//EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> AddComponentSparse(ComponentType&& component);

	private:

		WeakPtr<WorldEntity> m_World;
		GameObjectId m_Id;
	};

	//template <typename ComponentType>
	//EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> GameObject::AddComponent(ComponentType&& component)
	//{
	//	const ComponentTypeId newComponentTypeId{ BaseComponent::GetId<ComponentType>() };

	//	
	//}

	//template <typename ComponentType>
	//EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> GameObject::AddComponentSparse(
	//	ComponentType&& component)
	//{
	//	// TODO: do the component sparse
	//}
}


