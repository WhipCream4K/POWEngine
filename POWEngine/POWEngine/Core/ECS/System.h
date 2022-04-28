#pragma once
#include "Archetype.h"
#include "SystemBase.h"
#include "POWEngine/Core/Components/BaseComponent.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"

namespace powe
{
	template<typename ...Args>
	class System :
		public SystemBase
	{
	public:

		System() = default;
		virtual ~System() override = default;

		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> GetComponent(GameObjectID id);

		template<typename ...Args>
		std::tuple<std::add_pointer_t<Args>...> GetComponentsView(GameObjectID id);

	private:

		template<typename T>
		T* GetElement(const Archetype& archetype);

	};



	//template <typename ComponentType>
	//EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> System::GetComponent(GameObjectID id)
	//{
	//	const auto archetype{ m_World.GetArchetypeByGameObject(id) };
	//	const ComponentTypeID componentID{ BaseComponent::GetId<ComponentType>() };

	//	if (archetype)
	//	{
	//		return &archetype->ComponentData[m_UpdateCount * archetype->SizeOfComponentsBlock + archetype->ComponentOffsets.at(componentID)]

	//	}

	//	return nullptr;
	//}

	//template <typename ... Args>
	//std::tuple<std::add_pointer_t<Args>...> System::GetComponentsView(GameObjectID id)
	//{
	//	if (const auto archetype{ m_World.GetArchetypeByGameObject(id) })
	//	{
	//		return	std::make_tuple(GetElement<Args>(*archetype)...);
	//	}

	//	return std::tuple<std::add_pointer_t<Args>...>();
	//}

	//template <typename T>
	//T* System::GetElement(const Archetype& archetype)
	//{
	//	const ComponentTypeID componentID{ BaseComponent::GetId<T>() };
	//	return reinterpret_cast<T*>(&archetype.ComponentData[m_UpdateCount * archetype.SizeOfComponentsBlock + archetype.ComponentOffsets.at(componentID)]);
	//}
}


