#pragma once
#include "Archetype.h"
#include "SystemBase.h"
#include "POWEngine/Core/Components/BaseComponent.h"
#include "POWEngine/Core/WorldEntity/WorldEntity.h"

namespace powe
{
	class System :
		public SystemBase
	{
	public:

		System(WorldEntity& world, PipelineLayer layer,const std::vector<ComponentTypeID>& types);
		virtual ~System() override = default;

		virtual void OnUpdate(float,GameObjectID) {}

		template<typename ComponentType>
		EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> GetComponent(GameObjectID id);

		template<typename ...Args>
		std::tuple<Args...> GetArchetypeView(GameObjectID id);

	protected:

		void InternalUpdate(const Archetype& archetype, float deltaTime) override final;

	private:

	};

	template <typename ComponentType>
	EnableIsBasedOf<BaseComponent, ComponentType, ComponentType*> System::GetComponent(GameObjectID id)
	{
		const auto archetype{ m_World.GetArchetypeByGameObject(id) };
		const ComponentTypeID componentID{ BaseComponent::GetId<ComponentType>() };

		if(archetype)
		{
			SizeType accumulateOffset{};
			RawByte* startAddress{ &archetype->ComponentData[archetype->SizeOfComponentsBlock]};
			for (const auto& type: archetype->Types)
			{
				if (componentID == type)
					return reinterpret_cast<ComponentType*>(startAddress + accumulateOffset);

				accumulateOffset += m_World.GetComponentSize(type);
			}
		}

		return nullptr;
	}

	template <typename ... Args>
	std::tuple<Args...> System::GetArchetypeView(GameObjectID id)
	{
		const auto archetype{ m_World.GetArchetypeByGameObject(id) };
		if(archetype)
		{
			constexpr size_t argumentsCnt{ sizeof...(Args) };
			SizeType strideOffsets[argumentsCnt]{};
			for (size_t i = 0; i < archetype->Types.size(); ++i)
			{
			}
		}
	}
}


