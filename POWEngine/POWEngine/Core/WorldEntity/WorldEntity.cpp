#include "pch.h"
#include "WorldEntity.h"

#include "POWEngine/Core/ECS/Archetype.h"
#include "POWEngine/Core/ECS/SystemBase.h"

powe::WorldEntity::WorldEntity() = default;

void powe::WorldEntity::RemoveSystem(const SharedPtr<SystemBase>& system) const
{
	const auto& targetPipeline{ m_SystemPipeline[size_t(system->GetPipeLineLayer())] };
	const auto& itr = std::ranges::find(targetPipeline, system); // ranges::find since c++20
	if (itr != targetPipeline.end())
	{
		itr->get()->MarkDeleted(true);
	}
}

powe::GameObjectId powe::WorldEntity::GetEntityId()
{
	return m_GameObjectCounter++;
}

void powe::WorldEntity::UpdatePipeline(PipelineLayer layer, float deltaTime)
{
	const auto& systemsInThisPipeline{ m_SystemPipeline[size_t(layer)] };

	for (const auto& system : systemsInThisPipeline)
	{
		for (const auto& archetype : m_ArchetypesPool)
		{
			if(!archetype->GameObjectIds.empty())
			{
				if (IsDigitExistInNumber(archetype->Types, system->GetKeys()))
					system->InternalUpdate(*this, *archetype,deltaTime);
			}
		}
	}
}

bool powe::WorldEntity::IsDigitExistInNumber(const std::vector<ComponentTypeId>& compIds,
	const std::unordered_set<ComponentTypeId>& digit)
{
	for (const auto& id : compIds)
	{
		// this checks also take into account the hierarchy of the components
		const ComponentTypeId number{ id & ~(1u << ChildOfBitPos) }; // clearing the child of bit flag
		if (digit.contains(number))
			return true;
	}

	return false;
}

