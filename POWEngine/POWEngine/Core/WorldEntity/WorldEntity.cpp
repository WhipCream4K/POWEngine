#include "pch.h"
#include "WorldEntity.h"

#include "POWEngine/Core/ECS/SystemBase.h"

powe::WorldEntity::WorldEntity() = default;

void powe::WorldEntity::RemoveSystem(const SharedPtr<SystemBase>& system) const
{
	const auto& targetPipeline{ m_SystemPipeline[int(system->GetPipeLineLayer())] };
	const auto& itr = std::ranges::find(targetPipeline, system); // ranges::find since c++20
	if (itr != targetPipeline.end())
	{
		itr->get()->MarkDeleted(true);
	}
}
