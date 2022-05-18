#pragma once

#include "POWEngine/Core/CustomTypes.h"
#include "ECSTypes.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"
#include "POWEngine/ECS/ECSUtils.h"

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
		SystemBase(SystemBase&&) noexcept = delete;
		SystemBase& operator=(SystemBase&&) noexcept = delete;
		virtual ~SystemBase() = default;

	public:

		[[nodiscard]] const std::unordered_set<ComponentTypeID>& GetKeys() const { return m_Keys; }
		WorldEntity* GetWorld() const { return m_World; }

	protected:

		void InternalUpdate(const Archetype&, float);

		virtual void OnUpdate(float, powe::GameObjectID) = 0;
		virtual void OnCreate(GameObjectID) {}
		virtual void OnDestroy(GameObjectID) {}

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
		//try
		//{
		const ComponentTypeID compID{ BaseComponent::GetId<T>() };

		const auto findItr = archetype.ComponentOffsets.find(compID);

		if (findItr != archetype.ComponentOffsets.end())
		{
			RawByte* dataAddress{ &archetype.ComponentData[
				m_UpdateCountPerArchetype * archetype.SizeOfComponentsBlock
					+ findItr->second // offsets
					] };

			// check the key if it's a sparse component or not
			if (!IsThisComponentSparse(findItr->first))
			{
				return reinterpret_cast<T*>(dataAddress);
			}

			// if it is Sparse component
			// get the data from sparse section
			auto& sparseManager{ m_World->GetSparseComponentManager() };

			const SparseHandle* handle{ reinterpret_cast<SparseHandle*>(dataAddress) };
			RawByte* realCompData{ sparseManager.GetComponentData<T>(
				archetype.GameObjectIds[m_UpdateCountPerArchetype],compID,*handle) };

			//const SharedPtr<RawByte[]> sparseComponentData{
			//	sparseManager.GetComponentData(archetype.GameObjectIds[m_UpdateCountPerArchetype],compID) };

			return reinterpret_cast<T*>(realCompData);
		}

		throw std::out_of_range("no component in this archetype");

		//}
		//catch (const std::exception&)
		//{
		//	std::string errMsg{};
		//	errMsg.append("component name: ");
		//	errMsg.append(typeid(T).name());
		//	throw std::out_of_range(errMsg); // throws to update loop
		//}
	}

#pragma region MACRO

#define DEFINE_SYSTEM_KEY(...) InternMakeKeys<__VA_ARGS__>()

#pragma endregion
}


