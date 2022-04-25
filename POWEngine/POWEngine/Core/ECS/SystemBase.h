#pragma once

#include "ECSTypes.h"
#include "POWEngine/Core/WorldEntity/PipelineLayer.h"

#define DEFINE_SYSTEM_KEY(...)

//#define GET_SYSKEY() typeid(*this).name()::

//template<typename ...Args>
//struct 
//{
//	std::unordered_set<ComponentTypeID> MakeSystemKey()
//	{
//		return {};
//	}
//};

namespace powe
{
	struct Archetype;
	class WorldEntity;
	class SystemBase
	{
	public:



		explicit SystemBase(WorldEntity& world, PipelineLayer layer);
		SystemBase(const SystemBase&) = default;
		SystemBase& operator=(const SystemBase&) = delete;
		SystemBase(SystemBase&&) = default;
		SystemBase& operator=(SystemBase&&) noexcept = delete;
		virtual ~SystemBase() = default;

	public:

		void InternalUpdate(const Archetype& archetype, float deltaTime);

		[[nodiscard]] PipelineLayer GetPipeLineLayer() const { return m_Layer; }
		[[nodiscard]] const std::unordered_set<ComponentTypeID>& GetKeys() const { return m_Keys; }

		//void MarkDeleted(bool state) { m_MarkedDeleted = state; }

	protected:

		virtual void OnUpdate(float,GameObjectID) {}


		WorldEntity& m_World;
		PipelineLayer m_Layer;
		std::unordered_set<ComponentTypeID> m_Keys;
		uint32_t m_UpdateCountPerArchetype;

	};
}


