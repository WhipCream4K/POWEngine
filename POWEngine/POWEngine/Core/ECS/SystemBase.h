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

		SystemBase(const SharedPtr<WorldEntity>& world, PipelineLayer layer);
		SystemBase(const SystemBase&) = default;
		SystemBase& operator=(const SystemBase&) = default;
		SystemBase(SystemBase&&) = default;
		SystemBase& operator=(SystemBase&&) = default;
		virtual ~SystemBase() = default;

	public:

		// Should prefer this update because necessary arguments already in cache line
		virtual void InternalUpdate(WorldEntity& world, const Archetype& archetype) = 0;

		virtual void InternalUpdate(float deltaTime, const SharedPtr<Archetype>& archetype) = 0;

		[[nodiscard]] PipelineLayer GetPipeLineLayer() const { return m_Layer; }

	protected:

		std::unordered_set<ComponentTypeId> m_Keys;
		WeakPtr<WorldEntity> m_World;
		PipelineLayer m_Layer;
	};
}


