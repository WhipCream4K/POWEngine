#pragma once

#include "POWEngine/ECS/SystemBase.h"
#include "POWEngine/Logger/LoggerUtils.h"

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

	template<bool U>
	class IfBase{};

	template<>
	class IfBase<false> final
	{};

	template<>
	class IfBase<true>
	{};

	template<typename T>
	class RenderSystem : public RenderSystemBase , public IfBase<std::is_base_of_v<RenderAPI,T>>
	{
	public:

		void InternalDraw(const WorldEntity& worldEntity, const Archetype&, const RenderAPI&) final;
		virtual void OnDraw(const T& renderer,GameObjectID id) = 0;

	public:

		RenderSystem() = default;
		virtual ~RenderSystem() override = default;

	protected:

		const WorldEntity* GetWorld() const { return m_CurrentWorld; }

		// Specialize GetComponent from iteration
		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> GetComponent() const;

		// Specialize GetComponent from iteration
		template<typename ...Args>
		std::tuple<std::add_pointer_t<Args>...> GetComponentsView() const;


	private:

		template<typename U>
		U* GetComponent(const Archetype& archetype) const;

	private:

		const Archetype* m_CurrentArchetype{};
		const WorldEntity* m_CurrentWorld{};
		uint32_t m_UpdateCountPerArchetype{};
	};

	template <typename ... Args>
	void RenderSystemBase::InternMakeKeys()
	{
		m_Keys = { BaseComponent::GetId<Args>()... };
	}

	template <typename T>
	void RenderSystem<T>::InternalDraw(const WorldEntity& worldEntity, const Archetype& archetype, const RenderAPI& renderer)
	{
		m_CurrentArchetype = &archetype;
		m_UpdateCountPerArchetype = 0;
		m_CurrentWorld = &worldEntity;

		try
		{
			for (const GameObjectID gameObjectID : archetype.GameObjectIds)
			{
				OnDraw(static_cast<const T&>(renderer), gameObjectID);
				++m_UpdateCountPerArchetype;
			}
		}
		catch (const std::exception& e)
		{
			std::string errMsg{ "System trying to access component that doesn't exist in this archetype -> " };
			errMsg.append(e.what());
			POWLOGWARNING(errMsg);

			m_UpdateCountPerArchetype = 0;
			m_CurrentArchetype = nullptr;
			m_CurrentWorld = nullptr;
		}

		m_UpdateCountPerArchetype = 0;
		m_CurrentArchetype = nullptr;
		m_CurrentWorld = nullptr;
	}

	template <typename T>
	template <typename ... Args>
	std::tuple<std::add_pointer_t<Args>...> RenderSystem<T>::GetComponentsView() const
	{
		return std::make_tuple(GetComponent<Args>(*m_CurrentArchetype)...);
	}

	template <typename T>
	template <typename ComponentType>
	EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> RenderSystem<T>::GetComponent() const
	{
		return GetComponent<ComponentType>(*m_CurrentArchetype);
	}

	template <typename T>
	template <typename U>
	U* RenderSystem<T>::GetComponent(const Archetype& archetype) const
	{
		const ComponentTypeID compID{ BaseComponent::GetId<U>() };

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
				return reinterpret_cast<U*>(dataAddress);
			}

			// if it is Sparse component
			// get the data from sparse section
			auto& sparseManager{ m_CurrentWorld->GetSparseComponentManager() };

			RawByte* realCompData{ sparseManager.GetComponentData<U>(
				archetype.GameObjectIds[m_UpdateCountPerArchetype],compID) };


			return reinterpret_cast<U*>(realCompData);
		}

		throw std::out_of_range(typeid(U).name());
	}


// #pragma region MACRO
//
// #define DEFINE_SYSTEM_KEY(...) InternMakeKeys<__VA_ARGS__>()
//
// #pragma endregion
}


