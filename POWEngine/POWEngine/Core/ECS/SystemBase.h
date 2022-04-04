#pragma once

#include "ECSTypes.h"
#include "POWEngine/Core/WorldEntity/PipelineLayer.h"

namespace powe
{
	struct Archetype;
	class WorldEntity;
	class SystemBase
	{
	public:

		//explicit SystemBase(const SharedPtr<WorldEntity>& world, PipelineLayer layer);
		explicit SystemBase(WorldEntity& world, PipelineLayer layer,const std::vector<ComponentTypeID>& componentTypes);
		SystemBase(const SystemBase&) = default;
		SystemBase& operator=(const SystemBase&) = delete;
		SystemBase(SystemBase&&) = default;
		SystemBase& operator=(SystemBase&&) noexcept = delete;
		virtual ~SystemBase() = default;

	public:

		// Should prefer this update because necessary arguments already in cache line
		virtual void InternalUpdate(const Archetype& archetype,float deltaTime) = 0;

		//virtual void InternalUpdate(float deltaTime, const SharedPtr<Archetype>& archetype) = 0;

		[[nodiscard]] PipelineLayer GetPipeLineLayer() const { return m_Layer; }
		[[nodiscard]] const std::unordered_set<ComponentTypeID>& GetKeys() const { return m_Keys; }

		void MarkDeleted(bool state) { m_MarkedDeleted = state; }

	protected:

		//std::unordered_set<ComponentTypeID> m_Keys;
		WorldEntity& m_World;

		std::unordered_set<ComponentTypeID> m_Keys;
		PipelineLayer m_Layer;
		bool m_MarkedDeleted;
	};
}


