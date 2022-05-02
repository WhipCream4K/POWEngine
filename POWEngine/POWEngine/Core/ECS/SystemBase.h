#pragma once

#include "POWEngine/Core/CustomTypes.h"
#include "ECSTypes.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"

namespace powe
{
	struct Archetype;
	class WorldEntity;
	class SystemBase
	{
		friend class WorldEntity;

	public:

		SystemBase();
		SystemBase(const SystemBase&) = delete;
		SystemBase& operator=(const SystemBase&) = delete;
		SystemBase(SystemBase&&) = delete;
		SystemBase& operator=(SystemBase&&) noexcept = delete;
		virtual ~SystemBase() = default;

	public:

		[[nodiscard]] const std::unordered_set<ComponentTypeID>& GetKeys() const { return m_Keys; }
		WorldEntity* GetWorld() const { return m_World; }

	protected:

		void InternalUpdate(const Archetype&, float);
		virtual void OnUpdate(float, powe::GameObjectID) {}

		template<typename ...Args>
		void InternMakeKeys();

		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> GetComponent() const;

		template<typename ...Args>
		std::tuple<std::add_pointer_t<Args>...> GetComponentsView() const;

	private:

		void SetWorld(WorldEntity* world);

		template<typename T>
		T* GetComponent(const Archetype& archetype) const;


		WorldEntity* m_World; // using pointer to be more flexible. Doesn't really need smartpointer here there's no ownership changing anyway
		std::unordered_set<powe::ComponentTypeID> m_Keys;
		const Archetype* m_CurrentArchetype;
		uint32_t m_UpdateCountPerArchetype;

	};

	template <typename ... Args>
	void SystemBase::InternMakeKeys()
	{
		m_Keys = { BaseComponent::GetId<Args>()... };
	}

	template <typename ComponentType>
	EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> SystemBase::GetComponent() const
	{
		return GetComponent<ComponentType>(*m_CurrentArchetype);
	}

	template <typename ... Args>
	std::tuple<std::add_pointer_t<Args>...> SystemBase::GetComponentsView() const
	{
		return std::make_tuple(GetComponent<Args>(*m_CurrentArchetype)...);
	}

	template <typename T>
	T* SystemBase::GetComponent(const Archetype& archetype) const
	{
		try
		{
			const ComponentTypeID compID{ BaseComponent::GetId<T>() };
			return reinterpret_cast<T*>(&archetype.ComponentData[
				m_UpdateCountPerArchetype * archetype.SizeOfComponentsBlock +
					archetype.ComponentOffsets.at(compID)]); // there's a throw here
		}
		catch (const std::exception& e)
		{
			std::string errMsg{};
			errMsg.append("component name: " + typeid(T).name());
			throw std::out_of_range(errMsg); // throws to update loop
		}
	}

#pragma region MACRO

#define DEFINE_SYSTEM_KEY(...) InternMakeKeys<__VA_ARGS__>()

#pragma endregion
}


