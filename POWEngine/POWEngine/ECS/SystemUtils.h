#pragma once

//#include "SystemBase.h"

#include "POWEngine/ECS/ECSTypes.h"
#include "POWEngine/Core/Components/BaseComponent.h"

namespace powe
{
	class SystemUtils
	{
	public:

		const std::unordered_set<ComponentTypeID>& GetKeys() const { return m_Keys; }

		template<typename ...Args>
		void InternMakeKeys();

		// Specialize GetComponent from iteration
		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> GetComponent() const;

		// Specialize GetComponent from iteration
		template<typename ...Args>
		std::tuple<std::add_pointer_t<Args>...> GetComponentsView() const;


	private:

		std::unordered_set<ComponentTypeID> m_Keys;
	};

	template <typename ... Args>
	void SystemUtils::InternMakeKeys()
	{
		m_Keys = { BaseComponent::GetId<Args>()... };
	}
}


